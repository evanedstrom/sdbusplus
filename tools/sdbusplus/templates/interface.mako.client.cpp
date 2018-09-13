#include <${"/".join(interface.name.split('.') + [ 'client.hpp' ])}>
<%
    namespaces = interface.name.split('.')
    classname = namespaces.pop()

    def interface_instance():
        return "_".join(interface.name.split('.') + ['interface'])
%>
namespace sdbusplus
{
    % for s in namespaces:
namespace ${s}
{
    % endfor
namespace client
{

${classname}::${classname}(const char* service, const char* path)
        : _bus(std::make_shared<bus::bus>(sdbusplus::bus::new_default())),
          _service(service),
          _path(path)
{
}

###
### Methods
###
    % for m in interface.methods:
${ m.cpp_prototype(loader, interface=interface, ptype='client-cpp') }
    % endfor

###
### Properties
###
    % for p in interface.properties:
${ p.cpp_prototype(loader, interface=interface, ptype='client-cpps') }
    % endfor

###
### Signals
###
    // TODO: Signals

    % for e in interface.enums:

namespace
{
/** String to enum mapping for ${classname}::${e.name} */
static const std::tuple<const char*, ${classname}::${e.name}> \
mapping${classname}${e.name}[] =
        {
        % for v in e.values:
            std::make_tuple( "${interface.name}.${e.name}.${v.name}", \
                ${classname}::${e.name}::${v.name} ),
        % endfor
        };

} // anonymous namespace

auto ${classname}::convert${e.name}FromString(const std::string& s) ->
        ${e.name}
{
    auto i = std::find_if(
            std::begin(mapping${classname}${e.name}),
            std::end(mapping${classname}${e.name}),
            [&s](auto& e){ return 0 == strcmp(s.c_str(), std::get<0>(e)); } );
    if (std::end(mapping${classname}${e.name}) == i)
    {
        throw sdbusplus::exception::InvalidEnumString();
    }
    else
    {
        return std::get<1>(*i);
    }
}

std::string convertForMessage(${classname}::${e.name} v)
{
    auto i = std::find_if(
            std::begin(mapping${classname}${e.name}),
            std::end(mapping${classname}${e.name}),
            [v](auto& e){ return v == std::get<1>(e); });
    return std::get<0>(*i);
}
    % endfor

} // namespace client
    % for s in reversed(namespaces):
} // namespace ${s}
    % endfor
} // namespace sdbusplus
