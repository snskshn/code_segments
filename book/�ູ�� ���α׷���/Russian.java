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

	// n�� 1�� �� ������ �ݺ��Ѵ�.
	while (n >= 1)
	{
	    // n�� Ȧ���� ��쿡 ���ؼ� ���� m�� ���� ����� ���Ѵ�.
	    if (n%2 == 1)
	    {
		result = result + m;
	    }

	    // n�� 2�� ������ m�� 2�� ���Ѵ�.
	    n = n / 2;
	    m = m * 2;
	}

	return result;
    }
}
