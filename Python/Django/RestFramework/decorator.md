# decorator

REST framework에서 기본으로 제공하는 router외에 별도의 method를 추가하는 방법을 제공한다.

### @detail_route

`@detail_route` 데코레이터는 URL 패턴에 PK를 포함하고 단일 인스턴스를 처리할때 사용된다.

```python
from django.contrib.auth.models import User
from rest_framework import status
from rest_framework import viewsets
from rest_framework.decorators import detail_route, list_route
from rest_framework.response import Response
from myapp.serializers import UserSerializer, PasswordSerializer

class UserViewSet(viewsets.ModelViewSet):
    """
    A viewset that provides the standard actions
    """
    queryset = User.objects.all()
    serializer_class = UserSerializer

    @detail_route(methods=['post'])
    def set_password(self, request, pk=None):
        user = self.get_object()
        serializer = PasswordSerializer(data=request.data)
        if serializer.is_valid():
            user.set_password(serializer.data['password'])
            user.save()
            return Response({'status': 'password set'})
        else:
            return Response(serializer.errors,
                            status=status.HTTP_400_BAD_REQUEST)

    @list_route()
    def recent_users(self, request):
        recent_users = User.objects.all().order('-last_login')

        page = self.paginate_queryset(recent_users)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(recent_users, many=True)
        return Response(serializer.data)
```

데코레이터는 아래 예제와 같이 추가 인수를 포함하여 설정할 수 있다.

```python
@detail_route(methods=['post'], permission_classes=[IsAdminOrIsSelf])
def set_password(self, request, pk=None):
   ...
```

다음과 같은 url `^users/{pk}/set_password/$`로 실행이 가능하다.

### @list_route

`@list_route` 데코레이터는 개체의 목록을 처리할때 사용된다.
