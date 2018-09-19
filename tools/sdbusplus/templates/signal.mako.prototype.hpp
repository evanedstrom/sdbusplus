<%
    def parameters(defaultValue=False):
        return ",\n            ".\
            join([ parameter(p, defaultValue) for p in signal.properties ])

    def parameter(p, defaultValue=False):
        r = "%s %s" % (p.cppTypeParam(interface.name), p.camelCase)
        if defaultValue:
            r += default_value(p)
        return r

    def parameters_as_list(pre="", post=""):
        return ", ".join([ "%s%s%s" % (pre, p.camelCase, post)
                for p in signal.properties ])

    def parameters_types_as_list():
        return ", ".join([ p.cppTypeMessage(interface.name)
                for p in signal.properties ])

    def default_value(p):
        if p.defaultValue != None:
            return " = " + str(p.defaultValue)
        else:
            return ""

    def interface_name():
        return interface.name.split('.').pop()
%>
###
### Emit 'header'
###
    % if ptype == 'header':
        /** @brief Send signal '${signal.name}'
         *
         *  ${ signal.description.strip() }
    % if len(signal.properties) != 0:
         *
        % for p in signal.properties:
         *  @param[in] ${p.camelCase} - ${p.description.strip()}
        % endfor
    % endif
         */
        void ${ signal.camelCase }(
            ${ parameters(True) });
###
### Emit 'vtable'
###
    % elif ptype == 'vtable':
    vtable::signal("${signal.name}",
                   details::${interface_name()}::_signal_${signal.CamelCase }
                        .data()),
###
### Emit 'callback-cpp'
###
    % elif ptype == 'callback-cpp':
void ${interface_name()}::${ signal.camelCase }(
            ${ parameters() })
{
    using sdbusplus::server::binding::details::convertForMessage;

    auto& i = _${"_".join(interface.name.split('.'))}_interface;
    auto m = i.new_signal("${ signal.name }");

    m.append(${ parameters_as_list(pre="convertForMessage(", post=")") });
    m.signal_send();
}

namespace details
{
namespace ${interface_name()}
{
static const auto _signal_${ signal.CamelCase } =
    % if len(signal.properties) == 0:
        utility::tuple_to_array(std::make_tuple('\0'));
    % else:
        utility::tuple_to_array(message::types::type_id<
                ${ parameters_types_as_list() }>());
    % endif
}
}
###
### Emit 'client-headers'
###
    % elif ptype == 'client-headers':
        /** Subscribe to ${signal.CamelCase}
         *  @return - 0 on success, negative if failed.
         */
        int subscribe${signal.CamelCase}();

        /** Unsubscribe to ${signal.CamelCase}
         *  @return - 0 on success, negative if failed.
         */
        int unsubscribe${signal.CamelCase}();
###
### Emit 'client-cpps'
###
    % elif ptype == 'client-cpps':
int ${interface_name()}::subscribe${signal.CamelCase}()
{
    auto m = _bus->new_method_call("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "AddMatch");
    m.append(std::string("sender='") + _interface + "',type='signal',member='${signal.name}'");
    auto reply = _bus->call(m);
    if (reply.is_method_error())
        return -1;
    return 0;
}

int ${interface_name()}::unsubscribe${signal.CamelCase}()
{
    auto m = _bus->new_method_call("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "RemoveMatch");
    m.append(std::string("sender='") + _interface + "',type='signal',member='${signal.name}'");
    auto reply = _bus->call(m);
    if (reply.is_method_error())
        return -1;
    return 0;
}
    % endif
