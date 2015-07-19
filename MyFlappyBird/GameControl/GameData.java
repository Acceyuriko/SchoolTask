package GameControl;

/**
 * 记录游戏的一些信息
 * @author zec_iiii
 */
public class GameData
{

    public GameData()
    {
    }

    public boolean isBreakRecord()
    {
        return isBreakRecord_;
    }

    public void setBreakRecord(boolean flag)
    {
        isBreakRecord_ = flag;
    }

    public int getHighestMark()
    {
        return highestMark_;
    }

    public void setHighestMark(int mark)
    {
        highestMark_ = mark;
    }

    public int getStateOfGame()
    {
        return stateOfGame_;
    }

    public void setStateOfGame(int state)
    {
        stateOfGame_ = state;
    }

    public int getReferencePoint()
    {
        return referencePoint_;
    }

    public void setReferencePoint(int dot)
    {
        referencePoint_ = dot;
    }

    public int getMark()
    {
        return mark_;
    }

    public void setMark(int mark)
    {
        mark_ = mark;
    }

    public int getStateOfBird()
    {
        return stateOfBird_;
    }

    public void setStateOfBird(int state)
    {
        stateOfBird_ = state;
    }

    public int getEarth()
    {
        return earth_;
    }

    public void setEarth(int dot)
    {
        earth_ = dot;
    }

    public int getLeftPillarHeight()
    {
        return leftPillarHeight_;
    }

    public void setLeftPillarHeight(int height)
    {
        leftPillarHeight_ = height;
    }

    public int getRightPillarHeight()
    {
        return rightPillarHeight_;
    }

    public void setRightPillarHeight(int height)
    {
        rightPillarHeight_ = height;
    }

    public int getLeftPillarX()
    {
        return leftPillarX_;
    }

    public void setLeftPillarX(int dot)
    {
        leftPillarX_ = dot;
    }

    public int getRightPillarX()
    {
        return rightPillarX_;
    }

    public void setRightPillarX(int dot)
    {
        rightPillarX_ = dot;
    }

    public double getVerticalSpeedOfBird()
    {
        return verticalSpeedOfBird_;
    }

    public void setVerticalSpeedOfBird(double speed)
    {
        verticalSpeedOfBird_ = speed;
    }

    public long getLastTimeOfUp()
    {
        return lastTimeOfUp_;
    }

    public void setLastTimeOfUp(long time)
    {
        lastTimeOfUp_ = time;
    }

    public int getHeightOfBird()
    {
        return heightOfBird_;
    }

    public void setHeightOfBird(int height)
    {
        heightOfBird_ = height;
    }

    // 是否打破记录
    private boolean isBreakRecord_ = false;
    // 最高分
    private int highestMark_ = 0;
    // 当前游戏的状态
    private int stateOfGame_ = 0;
    // 当前小鸟的状态
    private int stateOfBird_ = 0;
    // 当前获得的分数
    private int mark_ = 0;
    // 计算分数时的参照坐标
    private int referencePoint_ = 0;
    // 草地的坐标
    private int earth_ = 0;
    // 右边柱子的位置
    private int rightPillarX_ = 0;
    private int rightPillarHeight_ = 0;
    // 左边柱子的位置
    private int leftPillarX_ = 0;
    private int leftPillarHeight_ = 0;
    // 小鸟垂直方向的速度
    private double verticalSpeedOfBird_ = 0d;
    // 上一次飞起的时间，用于计算当前高度和速度
    private long lastTimeOfUp_ = 0l;
    // 小鸟的高度
    private int heightOfBird_ = 0;

    // 游戏的状态
    public static final int IS_RUNNING = 0;
    public static final int IS_GUIDE = 1;
    public static final int IS_OVER = 2;
    public static final int IS_PAUSE = 4;
    public static final int IS_LOADING = 8;
    public static final int IS_DYING = 16;
    // 小鸟的状态
    public static final int BIRD_UP = 0;
    public static final int BIRD_MIDDLE = 1;
    public static final int BIRD_DOWN = 2;
}
