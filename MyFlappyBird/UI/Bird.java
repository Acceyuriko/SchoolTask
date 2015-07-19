package UI;

import GameControl.GameData;
import globalconst.ConstValue;
import java.awt.Graphics;

/**
 * 小鸟图层
 * @author zec_iiii
 */
public class Bird extends MainLayer
{
    Bird(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制小鸟
    @Override
    public void draw(Graphics g)
    {
        // 小鸟的图片和小鸟的状态有关
        switch (gameData_.getStateOfBird()) {
            case GameData.BIRD_UP:
                g.drawImage(
                        ImageSource.BIRD_UP,
                        ConstValue.BIRD_X,
                        gameData_.getHeightOfBird(),
                        ConstValue.BIRD_LENGTH,
                        ConstValue.BIRD_LENGTH,
                        null
                );
                break;
            case GameData.BIRD_MIDDLE:
                g.drawImage(
                        ImageSource.BIRD_MIDDLE,
                        ConstValue.BIRD_X,
                        gameData_.getHeightOfBird(),
                        ConstValue.BIRD_LENGTH,
                        ConstValue.BIRD_LENGTH,
                        null
                );
                break;
            case GameData.BIRD_DOWN:
                g.drawImage(
                        ImageSource.BIRD_DOWN,
                        ConstValue.BIRD_X,
                        gameData_.getHeightOfBird(),
                        ConstValue.BIRD_LENGTH,
                        ConstValue.BIRD_LENGTH,
                        null
                );
                break;
            default:
                break;
        }
    }
}
