## ModelSerializer, `읽기 전용` 또는 `쓰기 전용` 필드 속성 지정하기

### `읽기 전용` 속성 지정하기

```Python
class AccountSerializer(serializers.ModelSerializer):
    class Meta:
        model = Account
        fields = ('id', 'account_name', 'users', 'created')
        read_only_fields = ('account_name',)
```

### `쓰기 전용` 속성 지정하기

```Python
class CreateUserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ('email', 'username', 'password')
        extra_kwargs = {'password': {'write_only': True}}
```
