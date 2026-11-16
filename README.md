# MiniDB

A small student database written in C, built around a Binary Search Tree.

Each student is identified by a unique ID, which is used as the key in the tree.

The goal is to implement the data structure from scratch and use it as the foundation of a simple database with a web interface.

### Student records

Each record contains:

- ID
- First and last name
- Email
- Age
- Course
- GPA

### Implementation

The BST is implemented in C and currently supports:

- Node initialization
- Insertion
- Search by student ID
- Inorder traversal

### Project structure

```text
backend/
  include/
  src/
  tests/

frontend/
  css/
  js/

data/
third_party/
```

### Build

```bash
gcc -Wall -Wextra -std=c11 \
-Ibackend/include \
backend/src/student.c \
backend/src/bst.c \
backend/tests/test_bst.c \
-o test_bst
```

```bash
./test_bst
```

### Development

The project is still in development. Next steps include BST deletion, memory management, data persistence and the web interface.

### Stack

C · HTML · CSS · JavaScript
