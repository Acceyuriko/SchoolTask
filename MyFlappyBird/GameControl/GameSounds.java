package GameControl;

/**
 * 播放游戏中的声音
 * @author zec_iiii
 */
public class GameSounds implements Runnable
{
    // 构造函数，初始化参数
    public GameSounds(int times, String soundPath)
    {
        times_ = times;
        soundPath_ = soundPath;
    }

    @Override
    public void run()
    {
        soundPlayer_.play(soundPath_, times_);
    }

    private SoundPlayer soundPlayer_ = new SoundPlayer();
    private int times_ = 0;
    private String soundPath_ = null;
}
