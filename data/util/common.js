function setNodeValue(name, value) {
	if (Number.isInteger(value) && (name.startsWith('dc') || name.startsWith('charger'))) {
		value = value.toFixed(1);
	}
	var targets = $("[name='" + name + "'],[id='" + name + "']")
	targets.filter("div,span").html(value);
	targets.filter("input[type='checkbox']").prop("checked", value);
	targets.filter("select,input[type!='checkbox']").val(value);
	$("#" + name + "Level").val(value);
}
