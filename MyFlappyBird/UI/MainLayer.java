package UI;

import java.awt.Graphics;
import GameControl.GameData;

/**
 * UI图层的父类，共享一个GameData对象
 * @author zec_iiii
 */
public abstract class MainLayer
{
    protected MainLayer(int x, int y, int width, int height)
    {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
    }

    public void setGameData(GameData gameData)
    {
        gameData_ = gameData;
    }

    // 绘制图层
    public abstract void draw(Graphics g);

    protected GameData gameData_ = null;

    // 图层的坐标以及宽和高
    protected int x_;
    protected int y_;
    protected int width_;
    protected int height_;
}
