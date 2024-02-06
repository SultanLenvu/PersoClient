В пайплайне есть два этапа: сборка и деплой

## Сборка

1. Собираем клиент
```sh
cd ${{ gitea.workspace }} # Заходим в директорию с репозиторием
mkdir build				  # Создаём директорию для сборки
cd build				  # Переходим в неё
# Запускаем CMake, профиль сборки - с оптимизацией и отладочной информацией
# Генератор Ninja, чтобы CMake создал build.ninja, а не Makefile
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja ..
# Запускаем сборку. ninja - это как make, но собирает быстрее и параллельно
# по умолчанию
ninja
```

2. Собираем .deb - пакет
```sh
# Создаём базовую директорию. ${{ gitea.ref_name }} заменяется на название тэга
# т.е. версию
mkdir persoclient-${{ gitea.ref_name }}-1_amd64
# Устанавливаем программу в эту директорию как в корень операционной системы.
# DESTDIR - обычная практика для этого
DESTDIR=persoclient-${{ gitea.ref_name }}-1_amd64 ninja install
# Создаём поддиректорию DEBIAN, куда складываются метаданные пакета
mkdir persoclient_${{ gitea.ref_name }}-1_amd64/DEBIAN
# Копируем метаинформацию в файл control. sed подставит значение версии из тэга
sed -e "s/@VERSION@/${{ gitea.ref_name }}/g" < ../DEBIAN/control.in > persoclient_${{ gitea.ref_name }}-1_amd64/DEBIAN/control
# Добавляем скрипт, который будет запущен после установки/обновления пакета
cp ../DEBIAN/postinst persoclient_${{ gitea.ref_name }}-1_amd64/DEBIAN/
# Пакуем. Владельцем всего в пакете станет root
dpkg-deb --build --root-owner-group persoclient_${{ gitea.ref_name }}-1_amd64
```

## Деплой

Использовал [эту статью](https://habr.com/ru/articles/51427/)

1. Обновление репозитория
```sh
# Копируем наш пакет в пул пакетов
cp persoclient_${{ gitea.ref_name }}-1_amd64.deb /var/www/repo/pool/internal
# Переходим в корневую директорию репозитория пакетов
cd /var/www/repo/
# apt-ftparchive packages создаст список пакетов
apt-ftparchive packages pool/internal > dists/mantic/internal/binary-amd64/Packages
# Создаём описание репозитория: версия дистрибутива (mantic - кодовое имя Ubuntu 23.10)
echo "Archive: mantic" > dists/mantic/Release
echo "Suite: mantic" >> dists/mantic/Release
# Секция internal
echo "Components: internal" >> dists/mantic/Release
# Поддерживаем пакеты только для amd64
echo "Architectures: amd64" >> dists/mantic/Release
# Добавляем контрольные суммы файлов Packages
apt-ftparchive release dists/mantic >> dists/mantic/Release
```

2. Обновление клиентов
```
ansible-playbook -i inventory.ini playbook.yaml -u root
```
 - `inventory.ini` - список хостов
 - `playbook.yaml` - плейбук, описывает, что нужно обновить
 - `-u root` - заходить под рутом
