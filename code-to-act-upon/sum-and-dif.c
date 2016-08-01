int mul(int a, int b, double dd);

int sum(int a, int b)
{
    return a + b;
}

int difference(int a, int b)
{
    return a - mul(a, b + 1, 3.14);
}
