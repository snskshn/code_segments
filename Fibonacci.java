public class Fibonacci
{
    public static void main(String[] args)
    {
	int inputNumber = Integer.parseInt(args[0]);

	StringBuffer stringBuffer = new StringBuffer();

	for (int i = 1; i <= inputNumber; i++)
	{
	    stringBuffer.delete(0, stringBuffer.length());
	    stringBuffer.append("F(");
	    stringBuffer.append(i);
	    stringBuffer.append(") = ");

	    stringBuffer.append(fibonacci(i));

	    System.out.println(stringBuffer.toString());
	}
    }

    private static int fibonacci (int n)
    {
	if (n <= 2)
	{
	    return 1;
	}

	else
	{
	    return fibonacci (n - 1) + fibonacci (n - 2);
	}
    }
}
