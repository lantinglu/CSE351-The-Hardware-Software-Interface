/*
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s): Lanting Lu  
 * NetID(s): llu3
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  // TODO: implement this function
  for (int i = 0; i < NUM_AISLES; i++) {
    unsigned long* aisle = &(aisles[i]);
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      unsigned short id = get_id(aisle, j);
      int item_num = stockroom[id];
      int prev_items = num_items(aisle, j);
      add_items(aisle, j, item_num);
      int now_items = num_items(aisle, j);
      int diff = now_items - prev_items;
      stockroom[id] -= diff;
     }
   }
 }

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  // TODO: implement this function
  int total_fulfilled = 0;
  for (int i = 0; i < NUM_AISLES; i++){
  	unsigned long* aisle = &(aisles[i]);
    for(int j = 0; j < SECTIONS_PER_AISLE; j++){
      unsigned short curr_id = get_id(aisle, j);
      if(curr_id == id){
        int available_items = num_items(aisle, j);
        int items_remove;
        if (num > available_items) {
                items_remove = available_items;
            } else {
                items_remove = num;
        }
        remove_items(aisle, j, items_remove);
        total_fulfilled += items_remove;
        num -= items_remove;
      }
    }
  }
  if (num > 0) {
        int stock_val = stockroom[id];
        int items_stock = (stock_val >= num) ? num : stock_val;
        stockroom[id] -= items_stock;
        total_fulfilled += items_stock;
  }
  return total_fulfilled;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  // TODO: implement this function
   for (int i = 0; i < NUM_AISLES; i++){
        unsigned long* aisle = &(aisles[i]);
        for(int j = 0; j < SECTIONS_PER_AISLE; j++){
          unsigned short id_sec = get_id(aisle, j);
          if (id_sec == id){
            unsigned short num_item = num_items(aisle, j);
            if(num_item == 0){
              unsigned short* res = (unsigned short*)(aisle) + j;
              return res;
            }
          }
	      }
   }
  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  // TODO: implement this function
  unsigned short max;
  unsigned short* res;
  for (int i = NUM_AISLES - 1; i >= 0; i--){
    unsigned long* aisle = &(aisles[i]);
    for(int j = SECTIONS_PER_AISLE - 1; j >= 0; j--){
      unsigned short num_item = num_items(aisle, j);
      if (num_item >= max){
        max = num_item;
        res = (unsigned short*)(aisle) + j;
      }
    }
  }
  return res;
}
