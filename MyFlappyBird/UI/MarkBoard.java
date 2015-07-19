package UI;

import java.awt.Graphics;

/**
 * 分数板的图层
 * @author zec_iiii
 */
public class MarkBoard extends MainLayer
{
    public MarkBoard(int x, int y, int width, int height)
    {
        super(x, y, width, height);
    }

    // 绘制分数板
    @Override
    public void draw(Graphics g)
    {
        g.drawImage(
                ImageSource.MARK_BOARD,
                x_,
                y_,
                null
        );
        // 绘制当前分数
        drawCurrentMark(g, gameData_.getMark());
        // 绘制最高分
        drawHighestMark(g, gameData_.getHighestMark());
        // 绘制奖牌
        drawMedal(g);
        if (gameData_.isBreakRecord()) {
            g.drawImage(
                    ImageSource.NEW_RECORD,
                    x_ + 180,
                    y_ + 45,
                    null
            );
            g.drawImage(
                    ImageSource.NEW_RECORD,
                    x_ + 180,
                    y_ + 90,
                    null
            );
        }

    }

    // 绘制奖牌
    private void drawMedal(Graphics g)
    {
        // 根据分数范围绘制不同的奖牌
        if (gameData_.getMark() < 10) {
            g.drawImage(
                    ImageSource.BRONZE_MEDAL,
                    x_ + 35,
                    y_ + 50,
                    null
            );
        } else if (gameData_.getMark() < 40) {
            g.drawImage(
                    ImageSource.SILVER_MEDAL,
                    x_ + 35,
                    y_ + 50,
                    null
            );
        } else if (gameData_.getMark() < 100) {
            g.drawImage(
                    ImageSource.GOLD_MEDAL,
                    x_ + 35,
                    y_ + 50,
                    null
            );
        } else {
            g.drawImage(
                    ImageSource.PLATINUM_MEDAL,
                    x_ + 35,
                    y_ + 50,
                    null
            );
        }
    }

    // 绘制最高分
    private void drawHighestMark(Graphics g, int mark)
    {
        String strMark = String.valueOf(mark);
        for (int i = 0; i < strMark.length(); i++) {
            // 这里分数是右对齐的
            drawNumber(g, mark % 10, x_ + 247 - 25 * i, y_ + 95, 25, 35);
            mark /= 10;
        }
    }

    // 绘制本次分数
    private void drawCurrentMark(Graphics g, int mark)
    {
        String strMark = String.valueOf(mark);
        for (int i = 0; i < strMark.length(); i++) {
            // 这里分数是右对齐的
            drawNumber(g, mark % 10, x_ + 247 - 25 * i, y_ + 40, 25, 35);
            mark /= 10;
        }
    }

    // 绘制数字
    private void drawNumber(Graphics g, int number, int x, int y, int width, int height)
    {
        switch (number) {
            case 0:
                g.drawImage(ImageSource.NUM0, x, y, width, height, null);
                break;
            case 1:
                g.drawImage(ImageSource.NUM1, x, y, width, height, null);
                break;
            case 2:
                g.drawImage(ImageSource.NUM2, x, y, width, height, null);
                break;
            case 3:
                g.drawImage(ImageSource.NUM3, x, y, width, height, null);
                break;
            case 4:
                g.drawImage(ImageSource.NUM4, x, y, width, height, null);
                break;
            case 5:
                g.drawImage(ImageSource.NUM5, x, y, width, height, null);
                break;
            case 6:
                g.drawImage(ImageSource.NUM6, x, y, width, height, null);
                break;
            case 7:
                g.drawImage(ImageSource.NUM7, x, y, width, height, null);
                break;
            case 8:
                g.drawImage(ImageSource.NUM8, x, y, width, height, null);
                break;
            case 9:
                g.drawImage(ImageSource.NUM9, x, y, width, height, null);
                break;
        }
    }
}
