# How to Solve the Secret

## 1. Identify if `secret` is encoded

- For this, we wrote `file secret`
- The result was `secret: ASCII text`
    - Now we know that the file contains ASCII text
- Even so, we need to analyze the content by ourselves. For this, we can write `head secret`, which gives us

```console
H4sICEjNJmcAA3Byb2dyYW0A7Vx7cBvHed8DeCQokiD4FClKFmRLtvwgSFAyRVGWBYgPgRL1pEi9
dQSBI4EIrwIHWvQjVqz4QT9aOUldN5UjOZ5xk4zSuk2reOzapq2MLbdux+4j1ngyLRtHrRQ/Ssd2
LNuy0O/b2z3cHQHZk3Sm/2Al3Lffb7/v228fd9y92907e/p7LYJAeLCQmwlyUw4P5T0MP7ReEwGs
g1TA9WqyhBQDX6ST8xCPgZ6xEAO1MTkr02uwqnyD1WOg85kcp4KOFhF98Bjo0yXEQIkjqyfqeMLK
x+njzA9O9Xo0PyfDnR4D/QUrF6d6PaybmWaVn1ntMVCFyd9n0rMwPeJi+M0eA51mFcEpr88i9utg
/neweuRUc5/k1tvJ9HYyeU67mVy3Th7DwFkl+Lvkt4XpnVik8icWeQw0X35bQa+YfPXAm3kbyy9f
OzhY/XPK+1lLJDzSvrwlEmyOhGPpg80HO9qb25e7UnFXm+YX5oF9at2mQWI5RKYQ490H47WMx/Rn
zq5/6835T/16QfSJwb4Rec3wHT98ldsQmAxh8rxLEBa3k2x/ImuRcVCsEn7XLnhw/mTqvrJ89fBH
8KvOgY/nwbfmwTF/Zw78D/LI35UHfyQPHsiDL8iT7/o88rflwW/IY2cyj/zX4FdFGknI46E877+9
VN5OnqxS8Xu4gtS3faMUlJPyWDilyMntG7si8Zi83T8SkYkkjUXjMSml+JOKJKmiOQVH43ES9Uci
```

- The content is only alphanumeric characters with `+` and `/`. A *Base64* kind of file looks like this.

## 2. Decode the secret from the recognized file type

- We write `base64 -d secret > decoded_secret`.
    - `-d` flag tells Linux to decode the content of secret using *base64* format

## 3. Identify what the secret really is

- Now that we've decoded the secret, we need to check the type of the decoded file, for this do `file decoded_secret` inside the terminal, from which we get:

```console
decoded_secret: gzip compressed data, was "program", last modified: Sun Nov  3 01:09:28 2024, from Unix, original size modulo 2^32 94398
```

- Behind the layed of *Base64 encoding* we can see that its just a compressed archive that uses `gzip` as the outer compression layer. We can see its content without extraction using `file -z decoded_secret`, that outputs the following:

```console
decoded_secret: POSIX tar archive (GNU) (gzip compressed data, from Unix)
```

## 4. Extract the contents of decoded_secret

- Now that we've identified that `decoded_secret` is a compressed archive, we can decompress it using `tar xvzf decoded_secret`.
  - `tar`: Calls the `tar` command, which is used for creating, extracting, and managing tar archives.
  - `x`: Extracts the contents of the archive.
  - `v`: Displays the progress in the terminal (verbose mode), showing the names of files as they’re extracted.
  - `z`: Specifies that the archive is compressed with gzip (a `.gz` file).
  - `f`: Indicates that the following argument, `decoded_secret`, is the name of the file to operate on.
- After doing this, the terminal gives us the next output:

```console
87014aef1d77646b/
87014aef1d77646b/pendrive_image.bmp.gz
87014aef1d77646b/program.gz
```

- This means that the content of `decoded_secret` has been extracted to a folder called `87014aef1d77646b` with contents inside. These are two single compressed files that we can decompress with `gzip`, as follows:

  - `gzip -d 87014aef1d77646b/program.gz`
  - `gzip -d 87014aef1d77646b/pendrive_image.bmp.gz`


## 5. Analyze the executable

Before doing anything risky and compromise our system, we should check what the executable contains, for this we can do the following:

- `strings decompresed_secret`: displays any readable strings within the binary, which can sometimes provide hints about its functionality
- `objdump -x decompressed_secret`: To display information about sections, headers, and other low-level details
- `ldd decompressed_secret`: To check the shared libraries the executable depends on
- (Not recommended) `gdb decompressed_secret`: If you want to debug or step through the program’s execution, you can load it into the GNU debugger. This isnt recommended yet because you should never execute something without knowing what it does. 

From all of these options, the one that takes our attention is `ldd decompressed_secret`, which gives us:

```console
linux-vdso.so.1 (0x00007ffcec9ae000)
lib87014aef.so => not found
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000074c6d0200000)
/lib64/ld-linux-x86-64.so.2 (0x000074c6d044b000)
```

- Now we can see that there are no unresolved dependencies except one called `lib87014aef.so`. We need to figure out what this library is and how we can obtain it.

Here's where maybe we can use the hint written in the room where our team found the usb with the secret file...

```plaintext
Si a %0.3pi sumas 0.10 y multiplicas por 100, la verdad se revelará.
```

For what I understand, this means that we should do (3.14+0.10)*100 = 324

28470 + (64 * 28) = 15504
48184 + (64 * 35) = 50424
