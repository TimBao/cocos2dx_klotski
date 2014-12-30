/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.tim.game.klotski;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.os.Bundle;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.view.Gravity;
import android.util.Log;

import cn.waps.AdInfo;
import cn.waps.AppConnect;
import cn.waps.UpdatePointsNotifier;
import cn.waps.extend.AppDetail;
import cn.waps.extend.AppWall;
import cn.waps.extend.QuitPopAd;



public class klotski extends Cocos2dxActivity{
    private static Context mContext;
    private static Handler handler;

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        mContext = this;
        handler = new Handler() {

            @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                    case 0:
                        // 显示推荐列表（综合）
                        //AppConnect.getInstance(mContext).showOffers(mContext);
                        showBannerAd();
                        break;
                    case 1:
                        // 显示插屏广告
                        // 判断插屏广告是否已初始化完成，用于确定是否能成功调用插屏广告
                        boolean hasPopAd = AppConnect.getInstance(mContext)
                            .hasPopAd(mContext);
                        System.out.println("------------------" + hasPopAd);
                        if (hasPopAd) {
                            AppConnect.getInstance(mContext).showPopAd(mContext);
                            System.out.println("------执行了showPopAd--------");
                        }
                        break;
                    case 2:
                        // 显示推荐列表（软件）
                        AppConnect.getInstance(mContext).showAppOffers(mContext);
                        break;
                    case 3:
                        // 显示推荐列表（游戏）
                        AppConnect.getInstance(mContext).showGameOffers(mContext);
                        break;
                    case 4:
                        // 获取全部自定义广告数据
                        //Intent appWallIntent = new Intent(mContext, AppWall.class);
                        //mContext.startActivity(appWallIntent);
                        break;
                    case 5:
                        // 获取一条自定义广告数据
                        //AdInfo adInfo = AppConnect.getInstance(mContext)
                        //    .getAdInfo();
                        //AppDetail.getInstanct().showAdDetail(mContext, adInfo);
                        break;
                    case 6:
                        // 消费虚拟货币.
                        //AppConnect.getInstance(mContext).spendPoints(10,
                        //        WapsCocos2dxDemo01.this);
                        break;
                    case 7:
                        // 奖励虚拟货币
                        //AppConnect.getInstance(mContext).awardPoints(10,
                        //        WapsCocos2dxDemo01.this);
                        break;
                    case 8:
                        // 显示自家应用列表
                        //AppConnect.getInstance(mContext).showMore(mContext);
                        break;
                    case 9:
                        // 根据指定的应用app_id展示其详情
                        //AppConnect.getInstance(mContext).showMore(mContext,
                        //        "c8c3dab81e65e695020e69a74ccff196");
                        break;
                    case 10:
                        // 调用功能广告接口（使用浏览器接口）
                        //String uriStr = "http://www.baidu.com";
                        //AppConnect.getInstance(mContext).showBrowser(mContext,
                        //        uriStr);
                        break;
                    case 11:
                        // 用户反馈
                        AppConnect.getInstance(mContext).showFeedback(mContext);

                        break;

                    case 12:
                        // 退屏广告
                        QuitPopAd.getInstance().show(mContext);
                        break;
                    case 13:
                        showExitDialog();
                        break;

                    }
                }
        };

        if (detectOpenGLES20()) {
            AppConnect.getInstance("c7714e2521f888589c7e87fe1218ca3f",
                    "samsung", this);

            // 预加载插屏广告内容
            AppConnect.getInstance(this).initPopAd(this);
        }

    }

    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // Klotski should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        AppConnect.getInstance(this).close();
    }

    private boolean detectOpenGLES20() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x20000);
    }

    public static void showAdStatic(int adTag) {
        Message msg = handler.obtainMessage();
        msg.what = adTag; // 私有静态的整型变量，开发者请自行定义值
        handler.sendMessage(msg);
    }

    private void showBannerAd() {
        // 互动广告
        LinearLayout adBannerLayout = new LinearLayout(this);
        adBannerLayout.setOrientation(LinearLayout.HORIZONTAL);
        FrameLayout.LayoutParams lp_banner = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        // 设置adBannerLayout的悬浮位置,具体的位置开发者根据需要设置
        lp_banner.gravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL;
        this.addContentView(adBannerLayout, lp_banner);
        LinearLayout bannerLayout = new LinearLayout(this);
        adBannerLayout.addView(bannerLayout);
        // 添加互动广告
        AppConnect.getInstance(this).showBannerAd(this, bannerLayout);
    }

    private void showExitDialog() {
        new AlertDialog.Builder(this).setMessage("确认退出程序么?")
            .setNegativeButton("确认", new DialogInterface.OnClickListener() {
           @Override
           public void onClick(DialogInterface dialog, int which) {
              System.exit(1);
           }

           }).setPositiveButton("取消", new DialogInterface.OnClickListener() {
           @Override
           public void onClick(DialogInterface dialog, int which) {
           }

           }).create().show();
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
}
