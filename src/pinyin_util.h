/*
Copyright 2009 Ryan Homer

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef pinyin_util_h
#define pinyin_util_h

#include <utf8proc.h>
#include "u8_util.h"

int pinyin_subscript(uint32_t digit);
int homonym_frequency(const char *word, u8_state_t *state);
unsigned char pinyin_normalized_char(uint32_t ch, int *tone);
int pinyin_ordinal(uint32_t ch, utf8proc_bool ignore_umlaut);
void pinyin_next_word(const char *s, char *word, int *tone, u8_state_t *state);

#endif /* pinyin_util_h */
