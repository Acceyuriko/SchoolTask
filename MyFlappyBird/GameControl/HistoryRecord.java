package GameControl;

import java.io.*;

/**
 * 历史记录，记录最高分
 * @author zec_iiii
 */
public class HistoryRecord
{
    // 读取资源文件
    static {
        // 得到类路径
        String path = System.getProperty("java.class.path");
        // 得到类所在文件夹
        int firstIndex = path.lastIndexOf(System.getProperty("path.separator")) + 1;
        int lastIndex = path.lastIndexOf(File.separator);
        FILE_PATH = path.substring(firstIndex, lastIndex) + "/record.dat";
    }

    public HistoryRecord()
    {
    }

    // 读取数据
    public int loadRecord()
    {
        int highestMark = 0;
        try {
            // 读记录文件
            FileReader file = new FileReader(FILE_PATH);
            BufferedReader br = new BufferedReader(file);
            // 获得最高分
            highestMark = Integer.parseInt(br.readLine());
            br.close();
        }
        catch (Exception e) {
            // 若读取失败则重新创建记录文件
            try {
                FileWriter out = new FileWriter(FILE_PATH);
                // 初始化最高分记录为0
                out.write("0");
                out.flush();
                out.close();
            }
            catch (Exception ee) {
                ee.printStackTrace();
            }
        }
        return highestMark;
    }

    // 保存记录
    public void saveRecord(int highestMark)
    {
        try {
            FileWriter output = new FileWriter(FILE_PATH);
            output.write(String.valueOf(highestMark));
            output.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static final String FILE_PATH;
}
