# Hanyu Pinyin Comparator
## Introduction
This library sorts UTF-8 encoded [Hanyu Pinyin](https://en.wikipedia.org/wiki/Pinyin). For SQLite, the term "collation" is used. This library provides the function prototype needed for using its custom collation feature. It is able to handle the diacritical tone marks used in pinyin. The convention is to use the MACRON, ACUTE, CARON and GRAVE versions of the vowels to represent the four tones used in Mandarin Chinese.

The sort order is based on the [ABC dictionaries](http://wenlin.com/abc).

## Sorting Algorithm
It is important to note that most of the time you may _not_ want to use strict tone order. These exceptions are built into the algorithms. Here are some examples to illustrate.

### Normalized sorting
For the most part, sorting is based on the toneless versions of the word. That is, the tone marks are ignored.

    Mà
    Mán

Mà < Mán, because Ma < Man. Here, tone is ignored. This makes sense when looking for words in an index. One expects "ma" to come before "man". This is especially true for compound words or phrases.

    zhōnghuà
    Zhōnghuá mínzú

Normally, zhōnghuá < zhōnghuà. However, zhōnghuà < Zhōnghuá mínzú. Intuitively, this makes sense. Note also that case is ignored. That is, pinyin comparison (at least with this library) is case insensitive.

### Strict sorting
If the words being compared are identical in spelling, then the tones are taken into account.

    Má
    Mà

Here, Má < Mà because Má has the 2nd tone whereas Mà has the fourth.

NOTE: Even though a toneless word is sometimes classified as a 5th tone, its order is before the 1st tone. So,

    Ma (neutral tone)
    Mā (1st tone)
    Má (2nd tone)
    Mǎ (3rd tone)
    Mà (4th tone)
    Māma

Strictly speaking, in this example, "ma" as a standalone pinyin word without tone doesn't exist. You'll generally find it at the end of a compound word such as "Māma" in which case it'll be placed after "Mà" based on standard alphabetic order.

### Homonym Frequency
If two words are identical, a cardinal prefix can break the tie to give them a specific order. This is only useful if these otherwise identical pinyin words are associated to different Chinese characters, different entries in a database, etc. The intention of these "homonym frequency" prefixes are to mark which corresponding Chinese characters are used more frequently

    ¹shí 十
    ²shí 时
    ³shí 石
    ⁴shí 食

In Unicode lingo, these characters are SUPERSCRIPT ONE, SUPERSCRIPT TWO, etc. For numbers larger than 9, use a combination of the single digits. For example, for 10, use SUPERSCRIPT ONE followed by SUPERSCRIPT ZERO.

Superscript | Unicode ID
---|---
0|0x2070
1|0x00B9
2|0x00B2
3|0x00B3
4|0x2074
5|0x2075
6|0x2076
7|0x2077
8|0x2078
9|0x2079

## Building & Installing
Tarballs are not provided so you'll need to build it yourself. This project was developed under macOS so you'd need to adjust accordingly for other platforms. However, it uses the GNU Build System for cross-platform building. It is also configured for CMake, if that works better for you.

NOTE: This library depends on utf8proc. You need to have this installed on your system and `utf8proc.h` needs to be locatable as a system header file. On macOS, you can install this via brew with `brew install utf8proc`.

### Building with GNU Build Tools
You will need to have autoconf, automake and libtool installed. From the project root directory:

```
$ autoreconf --install
$ mkdir build
$ cd build
$ ../configure
$ make
$ make install
```

### Building with CMake
You will need to have cmake installed. Specify where your system include and library paths are for utf8proc.

```
$ export CFLAGS=-isystem\ /usr/local/include
$ export LD_LIBRARY_PATH=/usr/local/lib
```

Then, from the project root directory:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ make install
```

NOTE: Only the cmake configuration has tests.

## APIs

Public APIs are located in `pinyin_compare.h`.

    int pinyin_compare(void* data, int l1, const void* s1, int l2, const void* s2);

Compares two UTF-8 encoded pinyin strings. Returns an
#### Parameters
- **data** - Not used at this time but is required for interfacing with SQLite.
- **l1** - Length of the first string. Pass a negative number and it will be computed for you. Another SQLite requirement.
- **s1** - First string to compare in UTF-8 format.
- **l2** - Length of second string. Pass a negative number here too, if you want.
- **s2** - Second string to compare in UTF-8 format.

#### Returns
...an integer which is:
- **negative**, if s1 < s2
- **zero**, if s1 == s2
- **positive**, if s1 > s2

## Using in SQLite 3
This is an example of pinyin collation from the SQLite CLI

```
> .open /path/to/database.db
> .load libpinyincompare
> select pinyin from sometable order by pinyin collate pinyin;
```

For use in C code, reference the `sqlite3_create_collation` function at [Define New Collating Sequences](https://www.sqlite.org/c3ref/create_collation.html).
