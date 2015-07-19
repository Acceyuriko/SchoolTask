package GameControl;

import java.io.BufferedInputStream;
import java.io.InputStream;
import javax.sound.sampled.*;

/**
 * 播放声音
 * @author zec_iiii
 */
public class SoundPlayer
{
    public SoundPlayer()
    {
    }

    public void play(String soundPath, int times)
    {
        // 播放times次
        while (times > 0) {
            times--;
            try {
                // 获取声音文件资源
                InputStream in = this.getClass().getResourceAsStream(soundPath);
                AudioInputStream audio = AudioSystem.getAudioInputStream(new BufferedInputStream(in));
                // 获取声音文件格式
                AudioFormat format = audio.getFormat();
                DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);
                dataLine_ = (SourceDataLine) AudioSystem.getLine(info);
                dataLine_.open(format);
                dataLine_.start();

                int nByte;
                byte[] buffer = new byte[128];
                // 从audio中读取数据写入dataLine_中，即可以播放声音
                while ((nByte = audio.read(buffer, 0, 128)) != -1) {
                    dataLine_.write(buffer, 0, nByte);
                }
                audio.close();
                dataLine_.close();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    // 源数据行
    private SourceDataLine dataLine_ = null;
}
