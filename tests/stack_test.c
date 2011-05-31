#include <assert.h>
#include "test_helper.h"
#include "../src/stack.h"

void test_initialize_stack() {
  struct stack *stack;

  allocate_stack(&stack);
  initialize_stack(stack);

  assert(stack->card->value == NO_VALUE);
  assert(!stack->next);

  free_stack(stack);
}

void test_duplicate_stack() {
  struct stack *stack_0, *stack_1;
  struct card *card[5];
  const int begin_y = 5, begin_x = 10;

  allocate_stack(&stack_0);
  initialize_stack(stack_0);
  for (int i = 0; i < 5; i++) {
    allocate_card(&card[i]);
    set_card(card[i], i, SPADES, EXPOSED, begin_y, begin_x);
    push(&stack_0, card[i]);
  }
  stack_1 = duplicate_stack(stack_0);

  assert(stack_0 != stack_1);
  assert(stacks_equal(stack_0, stack_1));
}

void test_empty_on_empty_stack() {
  struct stack *stack;

  allocate_stack(&stack);
  initialize_stack(stack);

  assert(empty(stack));

  free_stack(stack);
}

void test_empty_on_non_empty_stack() {
  struct stack *stack;
  struct card *card;

  allocate_card(&card);
  initialize_card(card);
  set_card(card, ACE, SPADES, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  push(&stack, card);

  assert(!empty(stack));

  free_stack(stack);
}

void test_length() {
  struct stack *stack;
  struct card *card[4];

  allocate_stack(&stack);
  initialize_stack(stack);

  assert(length(stack) == 0);

  for (int i = 0; i < 4; i++) {
    allocate_card(&card[i]);
    initialize_card(card[i]);
    set_card(card[i], i, SPADES, EXPOSED, 0, 0);
    push(&stack, card[i]);
    assert(length(stack) == i + 1);
  }

  free_stack(stack);
}

void test_push_on_empty_stack() {
  struct stack *stack;
  struct card *card;

  allocate_card(&card);
  initialize_card(card);
  set_card(card, ACE, SPADES, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  push(&stack, card);

  assert(cards_equal(stack->card, card));
  assert(!stack->next);

  free_stack(stack);
}

void test_push_on_non_empty_stack() {
  struct stack *stack;
  struct card *card_0, *card_1;

  allocate_card(&card_0);
  allocate_card(&card_1);
  initialize_card(card_0);
  initialize_card(card_1);
  set_card(card_0, ACE, SPADES, EXPOSED, 0, 0);
  set_card(card_1, ACE, HEARTS, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  push(&stack, card_0);
  push(&stack, card_1);

  assert(cards_equal(stack->card, card_1));
  assert(cards_equal(stack->next->card, card_0));
  assert(!stack->next->next);

  free_stack(stack);
}

void test_push_null_on_empty_stack() {
  struct stack *stack, *old_stack;

  allocate_stack(&stack);
  initialize_stack(stack);
  old_stack = stack;
  push(&stack, NULL);

  assert(cards_equal(stack->card, old_stack->card));
  assert(!stack->next);

  free_stack(stack);
}

void test_push_null_on_non_empty_stack() {
  struct stack *stack, *old_stack;
  struct card *card;

  allocate_card(&card);
  initialize_card(card);
  set_card(card, ACE, SPADES, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  old_stack = duplicate_stack(stack);
  push(&stack, NULL);

  assert(cards_equal(stack->card, old_stack->card));
  assert(stacks_equal(stack->next, old_stack->next));

  free_stack(stack);
}

void test_pop_on_empty_stack() {
  struct stack *stack;
  struct card *popped_card;

  allocate_stack(&stack);
  initialize_stack(stack);
  popped_card = pop(&stack);

  assert(empty(stack));
  assert(!popped_card);

  free_stack(stack);
}

void test_pop_on_stack_with_one_element() {
  struct stack *stack;
  struct card *card, *popped_card;

  allocate_card(&card);
  initialize_card(card);
  set_card(card, ACE, SPADES, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  push(&stack, card);
  popped_card = pop(&stack);

  assert(empty(stack));
  assert(popped_card == card);

  free_stack(stack);
}

void test_pop_on_stack_with_more_than_one_element() {
  struct stack *stack, *old_stack_next;
  struct card *card[3], *popped_card;

  allocate_stack(&stack);
  initialize_stack(stack);
  for (int i = 0; i < 3; i++) {
    allocate_card(&card[i]);
    initialize_card(card[i]);
    set_card(card[i], ACE, SPADES, EXPOSED, 0, 0);
    push(&stack, card[i]);
  }
  old_stack_next = stack->next;
  popped_card = pop(&stack);

  assert(length(stack) == 2);
  assert(stack == old_stack_next);
  assert(popped_card == card[2]);

  free_stack(stack);
}

void test_reverse_on_empty_stack() {
  struct stack *stack, *old_stack, *reversed_stack;

  allocate_stack(&stack);
  initialize_stack(stack);
  old_stack = stack;
  reversed_stack = reverse(stack);

  assert(reversed_stack == old_stack);

  free_stack(stack);
}

void test_reverse_on_stack_with_one_element() {
  struct stack *stack, *old_stack, *reversed_stack;
  struct card *card;

  allocate_card(&card);
  initialize_card(card);
  set_card(card, ACE, SPADES, EXPOSED, 0, 0);

  allocate_stack(&stack);
  initialize_stack(stack);
  push(&stack, card);
  old_stack = stack;
  reversed_stack = reverse(stack);

  assert(reversed_stack == old_stack);

  free_stack(stack);
}

void test_reverse_on_stack_with_more_than_one_element() {
  struct stack *stack, *old_stack, *reversed_stack, *unreversed_stack;
  struct card *card[3];

  allocate_stack(&stack);
  initialize_stack(stack);
  for (int i = 0; i < 3; i++) {
    allocate_card(&card[i]);
    initialize_card(card[i]);
    set_card(card[i], TWO + i, DIAMONDS + i, EXPOSED, 0, 0);
    push(&stack, card[i]);
  }
  old_stack = duplicate_stack(stack);
  reversed_stack = reverse(stack);

  allocate_stack(&unreversed_stack);
  initialize_stack(unreversed_stack);
  for (int i = 0; i < 3; i++) {
    push(&unreversed_stack, pop(&reversed_stack));
  }

  assert(stacks_equal(unreversed_stack, old_stack));

  free_stack(reversed_stack);
  free_stack(stack);
}

void test_reverse_should_not_change_stack() {
  struct stack *stack, *old_stack, *old_stack_address;
  struct card *card[3];

  allocate_stack(&stack);
  initialize_stack(stack);
  for (int i = 0; i < 3; i++) {
    allocate_card(&card[i]);
    initialize_card(card[i]);
    set_card(card[i], TWO + i, DIAMONDS + i, EXPOSED, 0, 0);
    push(&stack, card[i]);
  }
  old_stack_address = stack;
  old_stack = duplicate_stack(stack);
  reverse(stack);

  assert(stack == old_stack_address);
  assert(stacks_equal(stack, old_stack));

  free_stack(stack);
}

void test_stack() {
  test_initialize_stack();

  test_duplicate_stack();

  test_empty_on_empty_stack();
  test_empty_on_non_empty_stack();

  test_length();

  test_push_on_empty_stack();
  test_push_on_non_empty_stack();
  test_push_null_on_empty_stack();

  test_pop_on_empty_stack();
  test_pop_on_stack_with_one_element();
  test_pop_on_stack_with_more_than_one_element();

  test_reverse_on_empty_stack();
  test_reverse_on_stack_with_one_element();
  test_reverse_on_stack_with_more_than_one_element();
  test_reverse_should_not_change_stack();
}