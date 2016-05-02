# QuerySet, filter에 비교 연산 처리하기

Less than or equal:

```python
User.objects.filter(field__lte=0)
```

Greater than or equal:

```python
User.objects.filter(field__gte=0)
```

Likewise, lt for less than and gt for greater than. You can find them all [in the documentation](https://docs.djangoproject.com/en/dev/ref/models/querysets/).
