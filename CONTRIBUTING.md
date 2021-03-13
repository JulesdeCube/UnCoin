# 👪 Contributing

This file is a synthesis of the base rule use in the repo.


## 📝Codding guide line

### #️⃣ Versioning

We use [Semantic Versioning](https://semver.org/) : `MAJOR.MINOR.PATCH`:

- `MAJOR` was incremented every **rewrite**
- `MINOR` was incremented every **new feature**
- `PATCH` was incremented every **new fix/patch**

Please modify the `Makefile` after each version change.



### 🌳 Branch structure

The main branch is `master`. The `master` is protected you can't push on it, you need to create a new branch and make a [pull request](https://github.com/JulesdeCube/UnCoin/pulls).

every branch need to follow this patern:
```
(feature|fix)/[a-z0-9-]+
```

### 📝 Commit

Each commit shoid follow this patern :
```markdown
ACTION: title

desciption

**ACTION_1:**
- `path/to/file1`: modification
  second line comments
- `path/to/file2`: modification

**ACTION_2:**
- `path/to/file2`: modification
```
`desciption` is optional

Where `ACTION` can be :
- `INIT`
- `ADD`
- `FIX`
- `MODIFY`
- `MOVE`
- `REMOVE`

you can add emoji in front of the title action and file path. You can use this emoji set: https://gitmoji.dev/

**Exemple:**
```markdown
✨ADD: mask object lib

add a lib to manage 1 channel images.
and other thing

**ADD:**
- ✨`src/image/mask.c`: the implementation of the `s_mask` struct
- ✨`src/image/mask.h`: the definition of the `s_mask` struct

**MODIFY:**
- 👷`Makefile`: add `image/mask` header and source file to
  compilation

**FIX:**
- 💚`Makefile`: create the parent forlder if not exist in `.o` file
````

### 🗂️ File structure

#### Root
- `/src/`: contained all the source (`.c`, `.h`, ...)
- `/build/`: the outputted binary (*ignored by git*)
- `/obj/`: compiled files (*ignored by git*)

#### Source
- `/src/client/`: the clients source files
- `/src/server/`: the server source files
- `/src/libs/`: all the different modules

#### Modules
- `/src/libs/<module_name>/tests/`: test for this modules
- `/src/libs/<module_name>/private/`: private methodes
- `/src/libs/<module_name>/public/`: public methodes
- `/src/libs/<module_name>/<module_name>.h`: file to import




### ✏️ Coding style

use the [**EPITA's C coding style** ](https://ceph.assistants.epita.fr/public-documents.assistants.epita.fr/coding-style-epita.pdf).

:heavy_plus_sign: All  code **MUST** be commented, otherwise you change can't be accepted and merge

### ✅ Test
we use cryterion to make test (see [epitech criterion](https://epitech-2022-technical-documentation.readthedocs.io/en/latest/criterion.html)).

### ⚒️Compilation

To add files into the compilation add they files path in the `SRC` variable of the [`Makefile`](./Makefile) for `.c` files and the `DEPS` variable for `.h` files.





## ↩️ Feedback guide line

If you have any problem first please search if the problem is not yet report.

If it is a new problem please create a [new issues](https://github.com/JulesdeCube/UnCoin/issues).



### Issues rules

- a clear description of the problem
- the using version
- a procedure to recreate the problem
- a video or an image if possible