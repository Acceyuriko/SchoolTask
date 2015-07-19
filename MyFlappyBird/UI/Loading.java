package UI;

import java.awt.Graphics;

/**
 * 载入时的画面
 * @author zec_iiii
 */
public class Loading extends MainLayer
{
    public Loading(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制loading图
    @Override
    public void draw(Graphics g)
    {
        g.drawImage(
                ImageSource.LOADING,
                x_,
                y_,
                384,
                512,
                null
        );
    }
}
