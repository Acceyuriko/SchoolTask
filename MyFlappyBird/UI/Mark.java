package UI;

import globalconst.ConstValue;

import java.awt.Graphics;

/**
 * 分数图层
 * @author zec_iiii
 */
public class Mark extends MainLayer
{
    public Mark(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制分数
    @Override
    public void draw(Graphics g)
    {
        int mark = gameData_.getMark();
        String strMark = String.valueOf(mark);
        // 计算分数最右边的坐标，以绘制分数,这里分数是居中绘制的
        int rightEnd = ConstValue.BACKGROUND_WIDTH / 2
                + ConstValue.NUMBER_WIDTH * strMark.length() / 2;
        for (int i = 0; i < strMark.length(); i++) {
            drawNumber(g, mark % 10, rightEnd - ConstValue.NUMBER_WIDTH * (i + 1), 100);
            mark /= 10;
        }
    }

    // 绘制数字
    private void drawNumber(Graphics g, int mark, int x, int y)
    {
        switch (mark) {
            case 0:
                g.drawImage(ImageSource.NUM0, x, y, null);
                break;
            case 1:
                g.drawImage(ImageSource.NUM1, x, y, null);
                break;
            case 2:
                g.drawImage(ImageSource.NUM2, x, y, null);
                break;
            case 3:
                g.drawImage(ImageSource.NUM3, x, y, null);
                break;
            case 4:
                g.drawImage(ImageSource.NUM4, x, y, null);
                break;
            case 5:
                g.drawImage(ImageSource.NUM5, x, y, null);
                break;
            case 6:
                g.drawImage(ImageSource.NUM6, x, y, null);
                break;
            case 7:
                g.drawImage(ImageSource.NUM7, x, y, null);
                break;
            case 8:
                g.drawImage(ImageSource.NUM8, x, y, null);
                break;
            case 9:
                g.drawImage(ImageSource.NUM9, x, y, null);
                break;
            default:
                break;
        }
    }
}
