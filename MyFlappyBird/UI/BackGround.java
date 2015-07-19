package UI;

import java.awt.Graphics;

/**
 * 背景图层
 * @author zec_iiii
 */
public class BackGround extends MainLayer
{
    public BackGround(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制背景图层
    @Override
    public void draw(Graphics g)
    {
        g.drawImage(
                ImageSource.BACKGROUND_BACK,
                0,
                0,
                384,
                448,
                null
        );
    }
}
