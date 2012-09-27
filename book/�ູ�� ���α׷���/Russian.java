public class Russian
{
    public static void main (String[] args)
    {
	int n = Integer.parseInt (args[0]);
	int m = Integer.parseInt (args[1]);
	StringBuffer stringBuffer = new StringBuffer ();
	stringBuffer.append (n);
	stringBuffer.append (" * ");
	stringBuffer.append (m);
	stringBuffer.append (" = ");
	stringBuffer.append (multiplyOfRussianPeasant(n, m));
	System.out.println (stringBuffer.toString());
    }

    private static int multiplyOfRussianPeasant (int n, int m)
    {
	int result = 0;

	// n이 1이 될 때까지 반복한다.
	while (n >= 1)
	{
	    // n이 홀수인 경우에 한해서 현재 m의 값을 결과에 더한다.
	    if (n%2 == 1)
	    {
		result = result + m;
	    }

	    // n은 2로 나누고 m은 2를 곱한다.
	    n = n / 2;
	    m = m * 2;
	}

	return result;
    }
}
