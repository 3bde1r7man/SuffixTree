# Suffix Tree Implementation
## Collaborators

- [Abdelrhman Mostafa](https://github.com/3bde1r7man)
- [Ahmed Hanfy](https://github.com/ahanfybekheet)
- [Marwan Tarik](https://github.com/MarwanTarik)
  
## Overview

This project aims to implement a suffix tree with specific requirements. The suffix tree will have the following characteristics:

1. Each node in the tree contains:
   - Linked list of children nodes (Method 2 in trie lecture).
   - The starting index (in the original string) of the characters on the edge going to this node.
   - The starting index (in the original string) of the suffix ending at this node (used for leaf nodes only, otherwise it should be -1).

2. The tree construction time complexity is O(n^2), and the tree space complexity is O(n), where n is the string length.

3. The search complexity is O(m+occ), where m is the length of the searched pattern, and occ is the number of occurrences of the pattern inside the string.
