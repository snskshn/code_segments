class Puzzle1
{
    public static void main(String[] args)
    {
	System.out.println(isOdd(-3));
	System.out.println(isOdd(-2));
	System.out.println(isOdd(-1));
	System.out.println(isOdd(0));
	System.out.println(isOdd(1));
	System.out.println(isOdd(2));
	System.out.println(isOdd(3));
    }

    public static boolean isOdd(int i)
    {
	return i % 2 == 1;
    }
}
