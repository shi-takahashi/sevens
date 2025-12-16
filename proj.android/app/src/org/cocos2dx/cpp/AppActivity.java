/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

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
package org.cocos2dx.cpp;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Build;
import android.view.WindowManager;
import androidx.annotation.NonNull;
import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.RequestConfiguration;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.firestore.AggregateQuery;
import com.google.firebase.firestore.AggregateSource;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.Query;
import com.google.firebase.firestore.SetOptions;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class AppActivity extends Cocos2dxActivity {

    private static final String TAG = "AppActivity";
    private static InterstitialAd interstitialAd;
    private static Context context = null;
    private static SharedPreferences sharedPref;
    private static FirebaseAnalytics firebaseAnalytics;
    private static FirebaseFirestore db;

    private static native void resumeNext();

    // Firestore callbacks to C++
    private static native void onRankingLoaded(String jsonData);
    private static native void onRankingLoadFailed();
    private static native void onMyRankLoaded(int rank);
    private static native void onMyRankLoadFailed();
    private static native void onScoreSaved(String odId);
    private static native void onScoreSaveFailed();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // Make sure we're running on Pie or higher to change cutout mode
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            // Enable rendering into the cutout area
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }
        // DO OTHER INITIALIZATION BELOW

        context = this;

        // Initialize Firebase Analytics
        firebaseAnalytics = FirebaseAnalytics.getInstance(this);
        Log.d(TAG, "Firebase Analytics initialized");

        // Initialize Firestore
        db = FirebaseFirestore.getInstance();
        Log.d(TAG, "Firestore initialized");

        // Log app_start event for testing
        Bundle bundle = new Bundle();
        bundle.putString("test_param", "hello");
        firebaseAnalytics.logEvent("app_start", bundle);
        Log.d(TAG, "Sent app_start event");

        // Initialize Mobile Ads SDK
        MobileAds.initialize(this);

        // Set test device
        RequestConfiguration requestConfiguration = new RequestConfiguration.Builder()
                .setTestDeviceIds(Arrays.asList("EMULATOR"))
                .build();
        MobileAds.setRequestConfiguration(requestConfiguration);

        requestNewInterstitial();
    }

    private static void requestNewInterstitial() {
        if (context == null) return;

        AdRequest adRequest = new AdRequest.Builder().build();
        InterstitialAd.load(context, "ca-app-pub-4630894580841955/8749981826", adRequest,
                new InterstitialAdLoadCallback() {
                    @Override
                    public void onAdLoaded(@NonNull InterstitialAd ad) {
                        interstitialAd = ad;
                        interstitialAd.setFullScreenContentCallback(new FullScreenContentCallback() {
                            @Override
                            public void onAdDismissedFullScreenContent() {
                                interstitialAd = null;
                                requestNewInterstitial();
                                resumeNext();
                            }

                            @Override
                            public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
                                interstitialAd = null;
                            }
                        });
                    }

                    @Override
                    public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                        interstitialAd = null;
                    }
                });
    }

    public static void launchInterstitial() {
        if (context == null) return;

        final Cocos2dxActivity activity = (Cocos2dxActivity) context;
        activity.runOnUiThread(new Runnable() {
            public void run() {
                if (interstitialAd != null) {
                    interstitialAd.show(activity);
                } else {
                    requestNewInterstitial();
                    resumeNext();
                }
            }
        });
    }


    public static int getPreferencesInt(String key) {
        return sharedPref.getInt(key, 0);
    }

    public static void putPreferencesInt(String key, int value) {
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putInt(key, value);
        editor.commit();
    }

    // ========== Firebase Analytics ==========

    public static void logEvent(String eventName, String paramName, String paramValue) {
        if (firebaseAnalytics == null) return;
        Bundle bundle = new Bundle();
        if (paramName != null && paramValue != null) {
            bundle.putString(paramName, paramValue);
        }
        firebaseAnalytics.logEvent(eventName, bundle);
        Log.d(TAG, "Logged event: " + eventName);
    }

    // ========== Firestore Ranking ==========

    /**
     * Get top 100 rankings
     * Called from C++
     */
    public static void getRanking(final String odId) {
        if (db == null) {
            onRankingLoadFailed();
            return;
        }

        db.collection("rankings")
            .orderBy("score", Query.Direction.DESCENDING)
            .limit(100)
            .get()
            .addOnSuccessListener(querySnapshot -> {
                StringBuilder json = new StringBuilder("[");
                int rank = 1;
                boolean first = true;
                for (DocumentSnapshot doc : querySnapshot.getDocuments()) {
                    if (!first) json.append(",");
                    first = false;

                    String name = doc.getString("name");
                    Long score = doc.getLong("score");
                    String odIdField = doc.getId();
                    String isMyData = odIdField.equals(odId) ? "1" : "0";

                    json.append("{");
                    json.append("\"ranking\":\"").append(rank).append("\",");
                    json.append("\"name\":\"").append(escapeJson(name != null ? name : "")).append("\",");
                    json.append("\"win\":\"").append(score != null ? score : 0).append("\",");
                    json.append("\"is_my_data\":\"").append(isMyData).append("\"");
                    json.append("}");
                    rank++;
                }
                json.append("]");
                onRankingLoaded(json.toString());
            })
            .addOnFailureListener(e -> {
                Log.e(TAG, "getRanking failed", e);
                onRankingLoadFailed();
            });
    }

    /**
     * Get my rank (count users with higher score + 1)
     * Called from C++
     */
    public static void getMyRank(final String odId) {
        if (db == null || odId == null || odId.isEmpty()) {
            onMyRankLoadFailed();
            return;
        }

        // First get my score
        db.collection("rankings").document(odId).get()
            .addOnSuccessListener(documentSnapshot -> {
                if (!documentSnapshot.exists()) {
                    onMyRankLoaded(0); // Not ranked yet
                    return;
                }

                Long myScore = documentSnapshot.getLong("score");
                if (myScore == null) {
                    onMyRankLoaded(0);
                    return;
                }

                // Count users with higher score
                AggregateQuery countQuery = db.collection("rankings")
                    .whereGreaterThan("score", myScore)
                    .count();

                countQuery.get(AggregateSource.SERVER)
                    .addOnSuccessListener(aggregateQuerySnapshot -> {
                        int rank = (int) aggregateQuerySnapshot.getCount() + 1;
                        onMyRankLoaded(rank);
                    })
                    .addOnFailureListener(e -> {
                        Log.e(TAG, "getMyRank count failed", e);
                        onMyRankLoadFailed();
                    });
            })
            .addOnFailureListener(e -> {
                Log.e(TAG, "getMyRank get doc failed", e);
                onMyRankLoadFailed();
            });
    }

    /**
     * Save my score
     * Called from C++
     * @param odId Document ID (empty string for new user)
     * @param name Player name
     * @param score Win count
     */
    public static void setMyRank(final String odId, final String name, final int score) {
        if (db == null) {
            onScoreSaveFailed();
            return;
        }

        Map<String, Object> data = new HashMap<>();
        data.put("name", name);
        data.put("score", score);
        data.put("updatedAt", com.google.firebase.Timestamp.now());

        DocumentReference docRef;
        if (odId == null || odId.isEmpty()) {
            // New user - create new document
            docRef = db.collection("rankings").document();
        } else {
            // Existing user - update document
            docRef = db.collection("rankings").document(odId);
        }

        docRef.set(data, SetOptions.merge())
            .addOnSuccessListener(aVoid -> {
                onScoreSaved(docRef.getId());
            })
            .addOnFailureListener(e -> {
                Log.e(TAG, "setMyRank failed", e);
                onScoreSaveFailed();
            });
    }

    private static String escapeJson(String s) {
        if (s == null) return "";
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t");
    }
}
