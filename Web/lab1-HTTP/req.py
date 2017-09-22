import requests

print('OPTIONS')
print('\n')

print('https://mail.ru')
print('\n')
r = requests.options('https://mail.ru')
print(r.status_code)
print(r.headers)

print('\n')
print('https://ya.ru')
print('\n')
r = requests.options('https://ya.ru')
print(r.status_code)
print(r.headers)

print('\n')
print('https://www.rambler.ru')
print('\n')
r = requests.options('https://www.rambler.ru')
print(r.status_code)
print(r.headers)

print('https://www.google.ru')
r = requests.options('https://www.google.ru')
print(r.status_code)
print(r.headers)

print('\n')
print('https://github.com')
print('\n')
r = requests.options('https://github.com')
print(r.status_code)
print(r.headers)

#print('https://ru.linkedin.com')
#r = requests.options('https://ru.linkedin.com')
#print(r.status_code)
#print(r.headers)

print('\n')
print('https://www.apple.com')
print('\n')
r = requests.options('https://www.apple.com/')
print(r.status_code)
print(r.headers)

print('\n')
print('HEAD')
print('\n')

print('https://vk.com')
print('\n')
r = requests.head('https://vk.com')
print(r.status_code)
print(r.headers)

print('\n')
print('https://www.apple.com')
print('\n')
r = requests.head('https://www.apple.com')
print(r.status_code)
print(r.headers)

print('\n')
print('http://www.msn.com/ru-ru/')
print('\n')
r = requests.head('http://www.msn.com/ru-ru/')
print(r.status_code)
print(r.headers)

print('\n')
print('GET')
print('\n')

print('https://yandex.ru/')
print('\n')
r = requests.get('https://yandex.ru/')
print(r.status_code)
print(r.headers)
print(r.text)

print('\n')
print('https://www.google.ru/')
print('\n')
r = requests.get('https://www.google.ru/')
print(r.status_code)
print(r.headers)
print(r.text)

print('\n')
print('POST')
print('\n')

print('https://yandex.ru/')
print('\n')
r = requests.post('https://yandex.ru/')
print(r.status_code)
print(r.headers)
print(r.text)

print('\n')
print('https://www.google.ru/')
print('\n')
r = requests.post('https://www.google.ru/')
print(r.status_code)
print(r.headers)
print(r.text)

print('\n')
print('API VK')
print('\n')

print('1')
r = requests.get('https://api.vk.com/method/users.get?user_ids=1&fields=sex,bdate,photo_max_orig,education&v=5.68')
print(r.status_code)
print(r.headers)
print(r.text)
print('\n')

print('2')
r = requests.get('https://api.vk.com/method/wall.get?domain=kolchak&access_token=9671fb97184e9386e9d5019c2786666795998b0fa9cfa9fd14929f8c54968d3cf5cb56ed7c90ee5eb414d&v=5.68&count=10')
print(r.status_code)
print(r.headers)
print(r.text)
print('\n')

print('3')
r = requests.get('https://api.vk.com/method/database.getCities?access_token=9671fb97184e9386e9d5019c2786666795998b0fa9cfa9fd14929f8c54968d3cf5cb56ed7c90ee5eb414d&v=5.68&country_id=1&need_all=0')
print(r.status_code)
print(r.headers)
print(r.text)
print('\n')

print('4')
r = requests.get('https://api.vk.com/method/database.getFaculties?access_token=9671fb97184e9386e9d5019c2786666795998b0fa9cfa9fd14929f8c54968d3cf5cb56ed7c90ee5eb414d&v=5.68&university_id=250')
print(r.status_code)
print(r.headers)
print(r.text)
print('\n')

print('PUT')
print('\n')

r = requests.put('https://jsonplaceholder.typicode.com/posts/1')
print(r.status_code)
print(r.headers)
print(r.text)

print('\n')
print('DELETE')
print('\n')

r = requests.delete('https://jsonplaceholder.typicode.com/posts/1')
print(r.status_code)
print(r.headers)
print(r.text)