TEMPLATE = subdirs

# Поддиректории проекта
SUBDIRS = \
    common \
    server \
    client

# Порядок сборки: сначала common, потом server и client
common.file = common/common.pro
server.file = server/server.pro
client.file = client/client.pro

server.depends = common
client.depends = common
