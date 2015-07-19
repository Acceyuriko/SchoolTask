package GameControl;

import UI.GameWindow;
import UI.GameContent;
import globalconst.ConstValue;

/**
 * 游戏主体
 * @author zec_iiii
 */
public class GameControl
{
    public GameControl()
    {
        // 实例化游戏数据
        gameData_ = new GameData();
        // 实例化游戏逻辑
        gameService_ = new GameService(gameData_);

        // 设置历史记录
        historyRecord_ = new HistoryRecord();
        gameData_.setHighestMark(historyRecord_.loadRecord());

        // 实例化游戏窗口
        gameContent_ = new GameContent(this, gameData_);
        new GameWindow(gameContent_);
        // 启动游戏
        gameRun();
    }

    public void gameRun()
    {
        // 按钮不可见，显示加载画面
        gameContent_.buttonTry_.setVisible(false);
        gameContent_.buttonRank_.setVisible(false);
        // 设置游戏正在加载
        gameData_.setStateOfGame(GameData.IS_LOADING);
        new LoadingThread().start();
    }

    // 加载进程
    private class LoadingThread extends Thread
    {
        @Override
        public void run()
        {
            try {
                this.sleep(2000);
                // 播放背景音乐
                new Thread(new GameSounds(9999, ConstValue.SOUND_BGM)).start();
                gameData_.setStateOfGame(GameData.IS_GUIDE);
                gameContent_.buttonTry_.setVisible(true);
                gameContent_.repaint();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    // 背景移动线程
    private class EarthMoveThread extends Thread
    {
        @Override
        public void run()
        {
            while (true) {
                if (gameData_.getStateOfGame() != GameData.IS_RUNNING) {
                    break;
                }
                // 重绘
                gameContent_.repaint();
                // 背景移动
                try {
                    sleep(30);
                    gameService_.earthMove();
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // 游戏结束时的线程
    private class GameOverThread extends Thread
    {
        @Override
        public void run()
        {
            // 小鸟下落到草地上
            while (gameData_.getHeightOfBird()
                    < ConstValue.BACKGROUND_HEIGHT - ConstValue.BIRD_LENGTH) {
                try {
                    sleep(30);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                gameService_.birdMove();
                // 重绘
                gameContent_.repaint();
            }

            // 设置游戏状态为死亡
            gameData_.setStateOfGame(GameData.IS_OVER);
            gameContent_.repaint();

            // 小鸟死亡后，按钮可见
            gameContent_.buttonTry_.setVisible(true);
            gameContent_.buttonRank_.setVisible(true);
        }
    }

    // 小鸟挥动翅膀的线程
    private class BirdWings extends Thread
    {
        @Override
        public void run()
        {
            while (true) {
                // 游戏不运行的时候小鸟不会挥动翅膀
                if (gameData_.getStateOfGame() != GameData.IS_RUNNING) {
                    break;
                }
                // 改变挥翅膀的状态
                gameService_.changeStateOfBird();
                try {
                    sleep(200);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // 游戏主线程
    private class MainThread extends Thread
    {
        @Override
        public void run()
        {
            // 启动游戏
            gameData_.setLastTimeOfUp(System.currentTimeMillis());

            earthMoveThread_ = new EarthMoveThread();
            earthMoveThread_.start();

            birdWings_ = new BirdWings();
            birdWings_.start();
            while (true) {
                try {
                    sleep(30);
                    // 若游戏结束则停止此线程
                    if (gameData_.getStateOfGame() == GameData.IS_DYING) {
                        gameOver();
                        break;
                    }

                    // 游戏非running时不进行任何操作
                    if (gameData_.getStateOfGame() != GameData.IS_RUNNING) {
                        continue;
                    }

                    // 判断草地移动线程是否已经启动，若未启动则启动之
                    if (!earthMoveThread_.isAlive()) {
                        earthMoveThread_ = new EarthMoveThread();
                        earthMoveThread_.start();
                    }
                    // 判断小鸟挥翅线程是否已经启动，若未启动则启动之
                    if (!birdWings_.isAlive()) {
                        birdWings_ = new BirdWings();
                        birdWings_.start();
                    }
                    // 运行游戏逻辑
                    gameService_.pillarMove();
                    gameService_.birdMove();
                    gameService_.checkCollision();
                    gameService_.checkGoal();

                    gameContent_.repaint();
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // 游戏开始时的操作
    public void start()
    {
        // 游戏逻辑开始运行
        gameService_.startGame();
        // 重绘
        gameContent_.repaint();

        // 运行主线程
        new MainThread().start();
    }

    // 处理键盘
    public void processKey(int keyCode)
    {
        // 处理键盘事件
        gameService_.responseToClick(keyCode);
    }

    // 游戏结束时的操作
    private void gameOver()
    {
        // 判断是否打破记录
        if (gameData_.getHighestMark() < gameData_.getMark()) {
            gameData_.setHighestMark(gameData_.getMark());
            gameData_.setBreakRecord(true);
            historyRecord_.saveRecord(gameData_.getHighestMark());
        }
        // 结束游戏
        new GameOverThread().start();
    }

    // 历史记录
    private HistoryRecord historyRecord_;
    // 游戏逻辑
    private GameService gameService_;
    // 小鸟挥翅线程
    private BirdWings birdWings_;
    // 草地移动线程
    private EarthMoveThread earthMoveThread_;
    // 游戏界面
    private GameContent gameContent_;
    // 游戏数据
    private GameData gameData_;
}
