/*
 * Copyright 2010-2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <aws/common/lifo_cache.h>
static int s_lifo_cache_put(struct aws_cache *cache, const void *key, void *p_value);
static void s_lifo_cache_clean_up(struct aws_cache *cache);

static struct aws_cache_vtable s_lifo_cache_vtable = {
    .clean_up = s_lifo_cache_clean_up,
    .find = aws_cache_base_find,
    .put = s_lifo_cache_put,
    .remove = aws_cache_base_remove,
    .clear = aws_cache_base_clear,
    .get_element_count = aws_cache_base_get_element_count,

    .use_lru_element = NULL,
    .get_mru_element = NULL,
};

struct aws_cache *aws_cache_new_lifo(
    struct aws_allocator *allocator,
    aws_hash_fn *hash_fn,
    aws_hash_callback_eq_fn *equals_fn,
    aws_hash_callback_destroy_fn *destroy_key_fn,
    aws_hash_callback_destroy_fn *destroy_value_fn,
    size_t max_items) {
    AWS_ASSERT(allocator);
    AWS_ASSERT(max_items);

    struct aws_lifo_cache *lifo_cache = aws_mem_calloc(allocator, 1, sizeof(struct aws_lifo_cache));
    if (!lifo_cache) {
        return NULL;
    }
    lifo_cache->base.allocator = allocator;
    lifo_cache->base.max_items = max_items;
    lifo_cache->base.vtable = &s_lifo_cache_vtable;
    if (aws_linked_hash_table_init(
            &lifo_cache->base.table, allocator, hash_fn, equals_fn, destroy_key_fn, destroy_value_fn, max_items)) {
        return NULL;
    }
    return &lifo_cache->base;
}

static void s_lifo_cache_clean_up(struct aws_cache *cache) {
    struct aws_lifo_cache *lifo_cache = AWS_CONTAINER_OF(cache, struct aws_lifo_cache, base);
    /* clearing the table will remove all elements. That will also deallocate
     * any cache entries we currently have. */
    aws_linked_hash_table_clean_up(&cache->table);
    aws_mem_release(cache->allocator, lifo_cache);
}

/* fifo cache put implementation */
static int s_lifo_cache_put(struct aws_cache *cache, const void *key, void *p_value) {
    if (aws_linked_hash_table_put(&cache->table, key, p_value)) {
        return AWS_OP_ERR;
    }

    /* Manage the space if we actually added a new element and the cache is full. */
    if (aws_linked_hash_table_get_element_count(&cache->table) > cache->max_items) {
        /* we're over the cache size limit. Remove whatever is in the one before the back of the linked_hash_table,
         * which was the latest element before we put the new one */
        const struct aws_linked_list *list = aws_linked_hash_table_get_iteration_list(&cache->table);
        struct aws_linked_list_node *node = aws_linked_list_back(list);
        if (!node->prev) {
            return AWS_OP_SUCCESS;
        }
        struct aws_linked_hash_table_node *table_node =
            AWS_CONTAINER_OF(node->prev, struct aws_linked_hash_table_node, node);
        return aws_linked_hash_table_remove(&cache->table, table_node->key);
    }

    return AWS_OP_SUCCESS;
}