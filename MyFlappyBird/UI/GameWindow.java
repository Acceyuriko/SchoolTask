package UI;


import javax.swing.JFrame;

/**
 * 游戏窗口
 * @author zec_iiii
 */
public class GameWindow extends JFrame
{
    public GameWindow(GameContent gameContent)
    {
        this.setTitle("myFlappyBird");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(388, 538);
        // 游戏窗口不可拉伸
        this.setResizable(false);
        // 窗口居中
        this.setLocationRelativeTo(null);
        // 设置游戏面板
        this.setContentPane(gameContent);
        this.setVisible(true);
    }

    private static final long serialVersionUID = 1L;
}
