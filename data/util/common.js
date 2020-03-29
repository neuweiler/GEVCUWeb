function setNodeValue(name, value) {
	var targets = $("[name='" + name + "'],[id='" + name + "']")
	targets.filter("div,span").html(value);
	targets.filter("input[type='checkbox']").prop("checked", value);
	targets.filter("select,input[type!='checkbox']").val(value);
	$("#" + name + "Level").val(value);
}
