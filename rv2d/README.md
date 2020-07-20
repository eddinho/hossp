# rv2d

This is a lightweight contiguous 2d array using an std::vector.

## Example

```c++
int main(int argc, char *argv[])
{
    rv2d::RowVector2D<int> rv2d(5, 5);

    for (int i = 0; i < rv2d.rows(); ++i)
    {
        for (int j = 0; j < rv2d.cols(); ++j)
        {
            rv2d[i][j] = i + j;
        }
    }

    for (int i = 0; i < rv2d.rows(); ++i)
    {
        for (int j = 0; j < rv2d.cols(); ++j)
        {
            std::cout << rv2d[i][j] << " " ;
        }
        std::cout << std::endl;
    }
}
```

Output

```bash
0 1 2 3 4 
1 2 3 4 5
2 3 4 5 6
3 4 5 6 7
4 5 6 7 8
```

## Linking

Header only library.