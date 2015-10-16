package yungyikim.demo_recyclerview_cardview;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        RecyclerView recyclerView = (RecyclerView)findViewById(R.id.recycler_view);

        // 콘텐츠가 변화해도 RecyclerView의 레이아웃 크기가 변경되지 않을 경우
        // 성능 향상을 위해 이 설정을 사용
        recyclerView.setHasFixedSize(true);

        // linear layout manager를 사용
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);

        List<RecyclerItem> items = new ArrayList<>();
        RecyclerItem[] item = new RecyclerItem[5];
        item[0] = new RecyclerItem(R.drawable.y1, "#1");
        item[1] = new RecyclerItem(R.drawable.y2, "#2");
        item[2] = new RecyclerItem(R.drawable.y2, "#3");
        item[3] = new RecyclerItem(R.drawable.y2, "#4");
        item[4] = new RecyclerItem(R.drawable.y2, "#5");

        for(int i=0;i<item.length;i++) items.add(item[i]);

        // adapter 지정 (다음 예제 참고)
        recyclerView.setAdapter(new RecyclerAdapter(getApplicationContext(), items, R.layout.activity_main));
    }

}
