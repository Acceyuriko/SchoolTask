package GameControl;

import globalconst.ConstValue;
import javafx.scene.input.KeyCode;

import java.awt.event.KeyEvent;

/**
 * 游戏逻辑
 * @author zec_iiii
 */
public class GameService
{
    public GameService(GameData gameData)
    {
        gameData_ = gameData;
    }

    // 反应点击操作
    public boolean responseToClick(int keyCode)
    {
        if (gameData_.getStateOfGame() == GameData.IS_RUNNING) {
            if (keyCode == KeyEvent.VK_SPACE) {
                // 播放声音
                new Thread(new GameSounds(1, ConstValue.SOUND_WING)).start();

                // 给小鸟一个向上的速度
                gameData_.setVerticalSpeedOfBird(ConstValue.BIRD_SPEED_WHEN_CLICK);
                // 记录此次操作的时间
                gameData_.setLastTimeOfUp(System.currentTimeMillis());

            }
            if (keyCode == KeyEvent.VK_P) {
                gameData_.setStateOfGame(GameData.IS_PAUSE);
                delta = System.currentTimeMillis() - gameData_.getLastTimeOfUp();
            }
        } else if (gameData_.getStateOfGame() == GameData.IS_PAUSE) {
            if (keyCode == KeyEvent.VK_P) {
                gameData_.setLastTimeOfUp(System.currentTimeMillis() - delta);
                gameData_.setStateOfGame(GameData.IS_RUNNING);
            }
        }

        return true;
    }

    // 草地移动
    public void earthMove()
    {
        int temp = gameData_.getEarth();
        // 每次移动4个像素，若超范围则清零
        if (temp >= ConstValue.BACKGROUND_WIDTH) {
            gameData_.setEarth(0);
        } else {
            gameData_.setEarth(temp + 4);
        }
    }

    // 柱子移动
    public void pillarMove()
    {
        // 柱子移动
        gameData_.setLeftPillarX(gameData_.getLeftPillarX() - 4);
        gameData_.setRightPillarX(gameData_.getRightPillarX() - 4);

        // 左边的柱子退出屏幕之后进行替换
        if (gameData_.getLeftPillarX() <= 0 - ConstValue.PILLAR_WIDTH) {
            gameData_.setLeftPillarX(gameData_.getRightPillarX());
            gameData_.setLeftPillarHeight(gameData_.getRightPillarHeight());
            gameData_.setRightPillarX(gameData_.getRightPillarX()
                    + ConstValue.PILLAR_WIDTH
                    + ConstValue.GAP_OF_HORIZONTAL_PILLAR);
            // 随机下一个柱子的高度
            gameData_.setRightPillarHeight((int) ((Math.random() * 1000) % 160 + 90));
        }

        // 设置小鸟得分的参考坐标
        gameData_.setReferencePoint(gameData_.getLeftPillarX() - ConstValue.BIRD_X);
    }

    // 小鸟的移动
    public void birdMove()
    {
        long currentTime = System.currentTimeMillis();

        double delta = currentTime - gameData_.getLastTimeOfUp();
        // 毫秒转换为秒
        delta /= 1000.0;

        gameData_.setLastTimeOfUp(currentTime);
        // 位移公式
        gameData_.setHeightOfBird((int) (
                        gameData_.getHeightOfBird()
                                + gameData_.getVerticalSpeedOfBird() * delta
                                + 0.5 * ConstValue.ACCELERATION * delta * delta)
        );
        // 速度公式
        gameData_.setVerticalSpeedOfBird(
                gameData_.getVerticalSpeedOfBird()
                        + ConstValue.ACCELERATION * delta
        );

    }

    // 改变小鸟的飞行状态
    public void changeStateOfBird()
    {
        int temp = gameData_.getStateOfBird();
        temp++;
        // 小鸟只有上中下三种状态
        if (temp == 3) {
            temp = 0;
        }
        gameData_.setStateOfBird(temp);
    }

    // 检测得分
    public void checkGoal()
    {
        int temp = gameData_.getReferencePoint();
        // 在宽度为4的区域内,因为柱子与背景的移动速度是4
        if (0 <= temp && temp <= 3) {
            gameData_.setMark(gameData_.getMark() + 1);
            // 播放得分声音
            new Thread(new GameSounds(1, ConstValue.SOUND_POINT)).start();
        }
    }

    // 碰撞检测
    public void checkCollision()
    {
        int currentHeightOfBird = gameData_.getHeightOfBird();
        int currentHeightOfPillar = gameData_.getLeftPillarHeight();

        // 检测是否碰到柱子
        if (currentHeightOfBird < currentHeightOfPillar
                || currentHeightOfBird > currentHeightOfPillar + ConstValue.GAP_OF_VERTICAL_PILLAR) {
            int currentPillarX = gameData_.getLeftPillarX();
            if (ConstValue.BIRD_X + ConstValue.BIRD_LENGTH > currentPillarX
                    && ConstValue.BIRD_X < currentPillarX + ConstValue.PILLAR_WIDTH) {
                // 碰到则结束游戏
                new Thread(new GameSounds(1, ConstValue.SOUND_HIT)).start();
                gameOver();
            }
        }
        // 若落到草地上则结束游戏
        if (currentHeightOfBird > ConstValue.BACKGROUND_HEIGHT - ConstValue.BIRD_LENGTH) {
            gameOver();
        }
    }

    // 开始游戏的初始化
    public void startGame()
    {
        gameData_.setHeightOfBird(ConstValue.BACKGROUND_HEIGHT / 2);
        gameData_.setMark(0);
        gameData_.setBreakRecord(false);
        gameData_.setVerticalSpeedOfBird(0);
        gameData_.setLastTimeOfUp(System.currentTimeMillis());
        gameData_.setLeftPillarHeight((int) ((Math.random() * 1000) % 160 + 90));
        gameData_.setRightPillarHeight((int) ((Math.random() * 1000) % 160 + 90));
        gameData_.setLeftPillarX(ConstValue.BACKGROUND_WIDTH);
        gameData_.setRightPillarX(ConstValue.BACKGROUND_WIDTH
                + ConstValue.GAP_OF_HORIZONTAL_PILLAR
                + ConstValue.PILLAR_WIDTH);

        gameData_.setStateOfGame(GameData.IS_RUNNING);
    }

    // 游戏结束时的操作
    private void gameOver()
    {
        // 播放结束音效
        new Thread(new GameSounds(1, ConstValue.SOUND_DIE)).start();
        gameData_.setVerticalSpeedOfBird(0);
        gameData_.setStateOfGame(GameData.IS_DYING);
    }

    private GameData gameData_;
    // 暂停时保留时间
    private long delta = 0;
}

