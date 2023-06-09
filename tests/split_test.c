/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/byte_buf.h>

#include <aws/testing/aws_test_harness.h>

AWS_TEST_CASE(test_char_split_happy_path, s_test_char_split_happy_path_fn)
static int s_test_char_split_happy_path_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = "testa;testb;testc";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(3, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));

    char *expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "testb";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_ends_with_token, s_test_char_split_ends_with_token_fn)
static int s_test_char_split_ends_with_token_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = "testa;testb;testc;";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(4, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    char *expected = "testa";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "testb";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 3));
    expected = "";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_begins_with_token, s_test_char_split_begins_with_token_fn)
static int s_test_char_split_begins_with_token_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = ";testa;testb;testc";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(4, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    char *expected = "";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testb";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 3));
    expected = "testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_token_not_present, s_test_char_split_token_not_present_fn)
static int s_test_char_split_token_not_present_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = "testa";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(1, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));

    char *expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_empty, s_test_char_split_empty_fn)
static int s_test_char_split_empty_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = "";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(1, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    ASSERT_INT_EQUALS(0, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_zeroed, s_test_char_split_zeroed_fn)
static int s_test_char_split_zeroed_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    struct aws_byte_cursor to_split;
    AWS_ZERO_STRUCT(to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(1, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    ASSERT_INT_EQUALS(0, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_adj_tokens, s_test_char_split_adj_tokens_fn)
static int s_test_char_split_adj_tokens_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = "testa;;testb;testc";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(4, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};
    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));

    char *expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testb";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 3));
    expected = "testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_with_max_splits, s_test_char_split_with_max_splits_fn)
static int s_test_char_split_with_max_splits_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    const char str_to_split[] = ";testa;testb;testc";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    ASSERT_SUCCESS(aws_array_list_init_dynamic(&output, allocator, 4, sizeof(struct aws_byte_cursor)));
    ASSERT_SUCCESS(aws_byte_cursor_split_on_char_n(&to_split, ';', 2, &output));
    ASSERT_INT_EQUALS(3, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    char *expected = "";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testb;testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    aws_array_list_clean_up(&output);

    return 0;
}

AWS_TEST_CASE(test_char_split_output_too_small, s_test_char_split_output_too_small_fn)
static int s_test_char_split_output_too_small_fn(struct aws_allocator *allocator, void *ctx) {
    (void)allocator;
    (void)ctx;

    const char str_to_split[] = "testa;testb;testc;";

    struct aws_byte_cursor to_split = aws_byte_cursor_from_c_str(str_to_split);

    struct aws_array_list output;
    struct aws_byte_cursor output_array[3] = {{0}};
    aws_array_list_init_static(&output, output_array, 3, sizeof(struct aws_byte_cursor));
    ASSERT_ERROR(AWS_ERROR_LIST_EXCEEDS_MAX_SIZE, aws_byte_cursor_split_on_char(&to_split, ';', &output));
    ASSERT_INT_EQUALS(3, aws_array_list_length(&output));

    struct aws_byte_cursor value = {0};

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 0));
    char *expected = "testa";

    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 1));
    expected = "testb";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    ASSERT_SUCCESS(aws_array_list_get_at(&output, &value, 2));
    expected = "testc";
    ASSERT_BIN_ARRAYS_EQUALS(expected, strlen(expected), value.ptr, value.len);

    return 0;
}

AWS_TEST_CASE(test_byte_cursor_next_split, s_test_byte_cursor_next_split)
static int s_test_byte_cursor_next_split(struct aws_allocator *allocator, void *ctx) {
    (void)allocator;
    (void)ctx;

    struct aws_byte_cursor to_split1 = AWS_BYTE_CUR_INIT_FROM_STRING_LITERAL("testa;testb;testc;a");
    struct aws_byte_cursor result1 = {0};
    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split1, ';', &result1));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result1, "testa");

    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split1, ';', &result1));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result1, "testb");

    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split1, ';', &result1));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result1, "testc");

    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split1, ';', &result1));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result1, "a");

    ASSERT_FALSE(aws_byte_cursor_next_split(&to_split1, ';', &result1));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result1, "");

    struct aws_byte_cursor to_split2 = AWS_BYTE_CUR_INIT_FROM_STRING_LITERAL("");
    struct aws_byte_cursor result2 = {0};
    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split2, ';', &result2));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result2, "");

    ASSERT_FALSE(aws_byte_cursor_next_split(&to_split2, ';', &result2));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result2, "");

    struct aws_byte_cursor to_split3 = AWS_BYTE_CUR_INIT_FROM_STRING_LITERAL(";;");
    struct aws_byte_cursor result3 = {0};
    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split3, ';', &result3));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result3, "");

    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split3, ';', &result3));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result3, "");

    ASSERT_TRUE(aws_byte_cursor_next_split(&to_split3, ';', &result3));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result3, "");

    ASSERT_FALSE(aws_byte_cursor_next_split(&to_split3, ';', &result3));
    ASSERT_CURSOR_VALUE_CSTRING_EQUALS(result3, "");

    return 0;
}
