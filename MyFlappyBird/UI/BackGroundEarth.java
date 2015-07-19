package UI;

import java.awt.*;

/**
 * 背景的草地
 * @author zec_iiii
 */
public class BackGroundEarth extends MainLayer
{
    public BackGroundEarth(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制草地
    @Override
    public void draw(Graphics g)
    {
        // 草地层是两张草地图片拼起来，以实现移动
        g.drawImage(
                ImageSource.BACKGROUND_EARTH,
                0,
                448,
                384 - gameData_.getEarth(),
                512,
                gameData_.getEarth(),
                0,
                384,
                64,
                null
        );
        g.drawImage(
                ImageSource.BACKGROUND_EARTH,
                384 - gameData_.getEarth(),
                448,
                384,
                512,
                0,
                0,
                gameData_.getEarth(),
                64,
                null
        );
    }
}
