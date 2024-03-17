# NumberSequencer

NumberSequencer is a utility that can be used to simplify creation of arrays filled with growing sequences of numbers at **compile time**. Requires C++20

## Usage
Include the header and use user defined literal provided `nseq` like so 

```cpp
constexpr auto seq = "1 2 3 ... (10)"_nseq;
```
Input known numbers `"1 2 3"`, separator `"."` (one is enough, three is used to make it look more similar to math notation) and the amount of unknown numbers to evaluate `"(10)"`
Return type is `std::array<std::int64_t, size>` where `size` will equal the amount of known numbers + unknown numbers (3 + 10). This can be evaluated automatically with `auto`
The return type should be declared as `constexpr` to process every step at compile time




Result:
```cpp
{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
```

You can determine complex sequences without information about the nature of the sequence itself, here's squared number sequence described in first terms

```cpp
constexpr auto seq = "1 4 9 16 ... (10)"_nseq;
```

Result:
```cpp
{1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196};
```

This sequence is more complicated, definition for it is a sum of squared and cubed numbers for each n

```cpp
constexpr auto seq = "2 12 36 80 150 ... (10)"_nseq;
```

Result:
```cpp
{2, 12, 36, 80, 150, 252, 392, 576, 810, 1100, 1452, 1872, 2366, 2940, 3600};
```

## Notes
- If the sequence is not growing constantly, algorithm will fail, so `"1 0 1 0 1"` will fail
- If the sequence referances itself, algorithm will fail, so `"0 1 1 2 3 5 7"` (Fibbonaci sequence) will fail. Might be fixed later
