package UI;

import globalconst.ConstValue;

import java.awt.Graphics;

/**
 * 游戏开始
 * @author zec_iiii
 */
public class GameStart extends MainLayer
{
    public GameStart(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制游戏开始界面
    @Override
    public void draw(Graphics g)
    {
        g.drawImage(
                ImageSource.START,
                x_,
                y_,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.BACKGROUND_HEIGHT,
                null
        );
    }
}
