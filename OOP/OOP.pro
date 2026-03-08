TEMPLATE = subdirs

# Поддиректории проекта
SUBDIRS = \
    common \
 #   server \
 #   client \
    function

# Порядок сборки: сначала common, потом остальные
common.file = common/common.pro
#server.file = server/server.pro
#client.file = client/client.pro
function.file = function/function.pro

#server.depends = common
#client.depends = common
function.depends = common
