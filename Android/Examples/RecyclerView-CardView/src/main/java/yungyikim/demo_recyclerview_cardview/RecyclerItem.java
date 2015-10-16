package yungyikim.demo_recyclerview_cardview;

/**
 * Created by yungyikim on 15. 10. 7..
 */
public class RecyclerItem {
    int image;
    String title;

    int getImage(){
        return this.image;
    }
    String getTitle(){
        return this.title;
    }

    RecyclerItem(int image, String title){
        this.image=image;
        this.title=title;
    }
}
