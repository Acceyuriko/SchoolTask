package UI;

import java.awt.Graphics;

/**
 * 游戏结束时的图层
 * @author zec_iiii
 */
public class GameOver extends MainLayer
{
    public GameOver(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }


    // 绘制游戏结束图片
    @Override
    public void draw(Graphics g)
    {
        g.drawImage(ImageSource.GAME_OVER, x_, y_, null);
    }
}
