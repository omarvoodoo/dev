#Serializers에서 일대 다 관계를 표현하기

Serializer 필드에 `many=True` 플레그를 추가하여 일대 다 관계를 표현 할 수있다.

##Example

###Serializer

다음은 Serializer 예제이다.

```python
class TrackSerializer(serializers.ModelSerializer):
    class Meta:
        model = Track
        fields = ('order', 'title')

class AlbumSerializer(serializers.ModelSerializer):
    tracks = TrackSerializer(many=True)

    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
```

중첩 된 관계를 직렬화하면 아래와 같을 것이다.

```python
{
    'album_name': 'The Grey Album',
    'artist': 'Danger Mouse',
    'tracks': [
        {'order': 1, 'title': 'Public Service Announcement'},
        {'order': 2, 'title': 'What More Can I Say'},
        {'order': 3, 'title': 'Encore'},
        ...
    ],
}
```

###Model

이번 예제의 모델은 `음악 앨범`이고 앨범에는 `트랙` 목록이 포함된다.

```python
class Album(models.Model):
    album_name = models.CharField(max_length=100)
    artist = models.CharField(max_length=100)

class Track(models.Model):
    album = models.ForeignKey(Album, related_name='tracks')
    order = models.IntegerField()
    title = models.CharField(max_length=100)
    duration = models.IntegerField()

    class Meta:
        unique_together = ('album', 'order')
        order_by = 'order'

    def __unicode__(self):
        return '%d: %s' % (self.order, self.title)
```

##References
* https://tomchristie.github.io/rest-framework-2-docs/api-guide/relations
