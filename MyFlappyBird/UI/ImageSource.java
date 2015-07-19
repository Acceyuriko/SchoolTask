package UI;

import java.awt.*;
import java.net.URL;
import javax.swing.*;

/**
 * 图片资源
 */
public class ImageSource
{
    public ImageSource()
    {
    }

    // 加载图片资源
    static {
        URL fileURL = ImageSource.class.getResource("/image/other/newrecord.png");
        NEW_RECORD = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/medal/platinum.png");
        PLATINUM_MEDAL = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/medal/gold.png");
        GOLD_MEDAL = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/medal/silver.png");
        SILVER_MEDAL = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/medal/bronze.png");
        BRONZE_MEDAL = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/markboard.png");
        MARK_BOARD = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/gameover.png");
        GAME_OVER = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/0.png");
        NUM0 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/1.png");
        NUM1 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/2.png");
        NUM2 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/3.png");
        NUM3 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/4.png");
        NUM4 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/5.png");
        NUM5 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/6.png");
        NUM6 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/7.png");
        NUM7 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/8.png");
        NUM8 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/number/9.png");
        NUM9 = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/background/background.png");
        BACKGROUND_BACK = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/background/earth.png");
        BACKGROUND_EARTH = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/erected.png");
        PILLAR_ERECTED = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/inverted.png");
        PILLAR_INVERTED = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/bird/up.png");
        BIRD_UP = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/bird/middle.png");
        BIRD_MIDDLE = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/bird/down.png");
        BIRD_DOWN = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/start.png");
        START = new ImageIcon(fileURL).getImage();

        fileURL = ImageSource.class.getResource("/image/other/retry.png");
        BUTTON_START = new ImageIcon(fileURL);

        fileURL = ImageSource.class.getResource("/image/other/rank.png");
        BUTTON_RANK = new ImageIcon(fileURL);

        fileURL = ImageSource.class.getResource("/image/other/loading.png");
        LOADING = new ImageIcon(fileURL).getImage();
    }

    public static final Image NEW_RECORD;
    public static final Image PLATINUM_MEDAL;
    public static final Image GOLD_MEDAL;
    public static final Image SILVER_MEDAL;
    public static final Image BRONZE_MEDAL;
    public static final Image MARK_BOARD;
    public static final Image GAME_OVER;
    public static final Image LOADING;
    public static final Image NUM0;
    public static final Image NUM1;
    public static final Image NUM2;
    public static final Image NUM3;
    public static final Image NUM4;
    public static final Image NUM5;
    public static final Image NUM6;
    public static final Image NUM7;
    public static final Image NUM8;
    public static final Image NUM9;
    public static final Image BACKGROUND_BACK;
    public static final Image BACKGROUND_EARTH;
    public static final Image PILLAR_ERECTED;
    public static final Image PILLAR_INVERTED;
    public static final Image BIRD_UP;
    public static final Image BIRD_MIDDLE;
    public static final Image BIRD_DOWN;
    public static final Image START;
    public static final ImageIcon BUTTON_START;
    public static final ImageIcon BUTTON_RANK;
}
