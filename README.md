# tc-naf

A toy compiler, part of a university project.

## External tools used

- [re2c](re2c.org)
- [The LEMON Parser Generator](www.hwaci.com/sw/lemon)
- [SS-CE](github.com/Sima214/SS-CE)
- [Jemalloc](jemalloc.net)

## State

It's mostly working.
Output is just an AST dump.

Example input:

```c
class Fibonacci {
    public static void main() {
        int first, second, i, tmp;
        first = 0;
        second = 1;
        i = 0;
        while (i < 10) {
            i = i + 1;
            tmp = first + second;
            println(tmp);
            first = second;
            second = tmp;
        }
    }
}
```

Example output:

```lua
Define function Fibonacci::main
  Block start:
    Allocating integers: [tmp:0, i:1, second:2, first:3]
    [*3]<-0
    [*2]<-1
    [*1]<-0
    While <cmp_l([*1], 10)>:
      Block start:
        [*1]<-add([*1], 1)
        [*0]<-add([*3], [*2])
        Calling <println>([*0])
        [*3]<-[*2]
        [*2]<-[*0]
      end
    end_while
  end
```

Tokenization happens with re2c and syntax parsing with Lemon. Semantic analysis happens at the second pass(also called validate troughtout the code). A third pass finally generates the output.
