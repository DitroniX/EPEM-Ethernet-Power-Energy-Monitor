import esphome.codegen as cg

CODEOWNERS = ["@DitroniXDev"]

atm90e36_ns = cg.esphome_ns.namespace("atm90e36")
ATM90E36Component = atm90e36_ns.class_("ATM90E36Component", cg.Component)

CONF_ATM90E36_ID = "atm90e36_id"
