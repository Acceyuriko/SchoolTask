package UI;

import java.awt.Graphics;
import java.awt.event.*;
import javax.swing.*;

import GameControl.*;
import globalconst.ConstValue;

/**
 * 控制UI绘制
 * @author zec_iiii
 */
public class GameContent extends JPanel
{
    public GameContent(GameControl gameControl, GameData gameData)
    {
        // 绑定控制对象
        gameControl_ = gameControl;
        gameData_ = gameData;

        // 设置布局
        this.setLayout(null);
        // 添加键盘监听器
        this.addKeyListener(new PlayerControl(gameControl));
        // 初始化各图层
        initLayer(gameData);
        // 初始化组件
        initComponent();
    }

    // 初始化各图层
    private void initLayer(GameData gameData)
    {
        // 实例化各图层
        // 背景
        backGround_ = new BackGround(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.WINDOW_HEIGHT
        );
        // 柱子
        pillar_ = new Pillar(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.BACKGROUND_HEIGHT
        );
        // 小鸟
        bird_ = new Bird(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.BACKGROUND_HEIGHT
        );
        // 引导界面
        gameStart_ = new GameStart(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.BACKGROUND_HEIGHT
        );
        // 草地
        backGroundEarth_ = new BackGroundEarth(
                0,
                ConstValue.BACKGROUND_HEIGHT,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.EARTH_HEIGHT
        );
        // 分数
        mark_ = new Mark(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.BACKGROUND_HEIGHT
        );
        // 装载图
        loading_ = new Loading(
                0,
                0,
                ConstValue.BACKGROUND_WIDTH,
                ConstValue.WINDOW_HEIGHT
        );
        // 小鸟死亡
        gameOver_ = new GameOver(
                64,
                100,
                0,
                0
        );
        // 分数板
        markBoard_ = new MarkBoard(
                42,
                180,
                ConstValue.MARKBOARD_WIDTH,
                ConstValue.MARKBOARD_HEIGHT
        );

        // 传递GameData数据
        backGround_.setGameData(gameData);
        pillar_.setGameData(gameData);
        bird_.setGameData(gameData);
        gameStart_.setGameData(gameData);
        backGroundEarth_.setGameData(gameData);
        mark_.setGameData(gameData);
        loading_.setGameData(gameData);
        gameOver_.setGameData(gameData);
        markBoard_.setGameData(gameData);
    }

    // 按钮组件
    private void initComponent()
    {
        // 开始按钮
        buttonTry_ = new JButton(ImageSource.BUTTON_START);
        // 刚开始的时候按钮在中间
        buttonTry_.setBounds(
                ConstValue.BACKGROUND_WIDTH / 2 - ConstValue.BUTTON_WIDTH / 2,
                370,
                ConstValue.BUTTON_WIDTH,
                ConstValue.BUTTON_HEIGHT
        );
        this.add(buttonTry_);
        buttonTry_.setBorder(null);

        buttonTry_.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                // 开始游戏
                gameControl_.start();
                // 隐藏两个按钮,并把开始按钮的位置左移
                buttonTry_.setBounds(34, 370, ConstValue.BUTTON_WIDTH, ConstValue.BUTTON_HEIGHT);
                buttonTry_.setVisible(false);
                buttonRank_.setVisible(false);
            }
        });

        buttonRank_ = new JButton(ImageSource.BUTTON_RANK);
        buttonRank_.setBounds(210, 370, ConstValue.BUTTON_WIDTH, ConstValue.BUTTON_HEIGHT);
        this.add(buttonRank_);
        buttonRank_.setBorder(null);
    }

    // 绘制各图层,回调函数
    @Override
    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);

        // 背景和草地是一直都要绘制的
        backGround_.draw(g);
        backGroundEarth_.draw(g);
        // 根据游戏进程绘制其他部分
        switch (gameData_.getStateOfGame()) {
            case GameData.IS_GUIDE:
                gameStart_.draw(g);
                break;
            case GameData.IS_LOADING:
                loading_.draw(g);
                break;
            case GameData.IS_PAUSE:
                pillar_.draw(g);
                mark_.draw(g);
                bird_.draw(g);
                break;
            case GameData.IS_RUNNING:
                pillar_.draw(g);
                mark_.draw(g);
                bird_.draw(g);
                break;
            case GameData.IS_OVER:
                gameOver_.draw(g);
                markBoard_.draw(g);
                break;
            case GameData.IS_DYING:
                pillar_.draw(g);
                mark_.draw(g);
                bird_.draw(g);
                break;
            default:
                break;
        }

        // 返回焦点
        this.requestFocus();
    }

    // 各图层引用
    private BackGround backGround_;
    private Pillar pillar_;
    private Bird bird_;
    private GameStart gameStart_;
    private BackGroundEarth backGroundEarth_;
    private Mark mark_;
    private Loading loading_;
    private GameOver gameOver_;
    private MarkBoard markBoard_;

    // 游戏控制部分
    private GameData gameData_;
    private GameControl gameControl_;

    // 按钮
    public JButton buttonTry_;
    public JButton buttonRank_;
}
