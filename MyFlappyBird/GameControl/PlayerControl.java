package GameControl;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 * 处理键盘事件
 * @author zec_iiii
 */
public class PlayerControl extends KeyAdapter
{
    public PlayerControl(GameControl gameControl)
    {
        gameControl_ = gameControl;
    }

    // 由gameControl_处理事件
    @Override
    public void keyPressed(KeyEvent e)
    {
        gameControl_.processKey(e.getKeyCode());
    }

    private GameControl gameControl_;
}
