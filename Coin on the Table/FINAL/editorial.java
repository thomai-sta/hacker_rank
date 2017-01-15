import java.util.Scanner;

public class CoinOnTheTable {

    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);

        int n = cin.nextInt();
        int m = cin.nextInt();
        int K = cin.nextInt();
        int x = 0, y = 0;
        char a[][] = new char[n][];
        for (int i=0; i<n; i++) {
            a[i] = cin.next().toCharArray();
            for (int j=0; j<m; j++)
                if (a[i][j] == '*') {
                    x = i;
                    y = j;
                }
        }


        int f[][][] = new int[K + 1][n][m];
        int ans = 1 << 29;
        for (int k=0; k<=K; k++)
            for (int i=0; i<n; i++)
                for (int j=0; j<m; j++)
                    if (k == 0) f[k][i][j] = i == 0 && j == 0 ? 0 : 1 << 29;
                    else {
                        int res = 1 << 29;
                        if (i > 0)     res = Math.min(res, f[k - 1][i - 1][j] + (a[i - 1][j] == 'D' ? 0 : 1));
                        if (i < n - 1) res = Math.min(res, f[k - 1][i + 1][j] + (a[i + 1][j] == 'U' ? 0 : 1));
                        if (j > 0)     res = Math.min(res, f[k - 1][i][j - 1] + (a[i][j - 1] == 'R' ? 0 : 1));
                        if (j < m - 1) res = Math.min(res, f[k - 1][i][j + 1] + (a[i][j + 1] == 'L' ? 0 : 1));

                        f[k][i][j] = res;
                    }

        for (int k=0; k<=K; k++)
            ans = Math.min(ans, f[k][x][y]);

        if (ans < (1 << 29)) System.out.println(ans);
        else System.out.println(-1);

        cin.close();
    }
}