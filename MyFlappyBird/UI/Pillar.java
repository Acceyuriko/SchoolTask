package UI;

import globalconst.ConstValue;

import java.awt.Graphics;

/**
 * 游戏中的柱子
 * @author zec_iiii
 */
public class Pillar extends MainLayer
{
    public Pillar(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    @Override
    public void draw(Graphics g)
    {
        // 绘制左边的柱子
        drawPillar(g, gameData_.getLeftPillarHeight(), gameData_.getLeftPillarX());
        // 绘制右边的柱子
        drawPillar(g, gameData_.getRightPillarHeight(), gameData_.getRightPillarX());
    }

    // 绘制柱子
    private void drawPillar(Graphics g, int height, int x)
    {
        // 倒立的柱子
        g.drawImage(
                ImageSource.PILLAR_INVERTED,
                x,
                0,
                x + ConstValue.PILLAR_WIDTH,
                height,
                0,
                ConstValue.PILLAR_HEIGHT - height,
                ConstValue.PILLAR_WIDTH,
                ConstValue.PILLAR_HEIGHT,
                null
        );
        // 正立的柱子
        g.drawImage(
                ImageSource.PILLAR_ERECTED,
                x,
                height + ConstValue.GAP_OF_VERTICAL_PILLAR,
                x + ConstValue.PILLAR_WIDTH,
                ConstValue.BACKGROUND_HEIGHT,
                0,
                0,
                ConstValue.PILLAR_WIDTH,
                ConstValue.BACKGROUND_HEIGHT - height - ConstValue.GAP_OF_VERTICAL_PILLAR,
                null
        );
    }
}
