/*
MIT License

scalu_packet.c
Copyright (c) 2020 Nick Trebes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <scalu/packet.h>

static void SCALU_CLS(packet,Packet)(lua_State *L);
static int SCALU_CLSFN(packet,Packet,convert_packet)(lua_State *L);
static int SCALU_CLSFN(packet,Packet,convert_packets)(lua_State *L);
static int SCALU_CLSFN(packet,Packet,lower_bonds)(lua_State *L);
static int SCALU_CLSFN(packet,Packet,upper_bonds)(lua_State *L);
static void SCALU_META(packet,Packet)(lua_State *L);
static int SCALU_METAFN(packet,Packet,add_payload)(lua_State *L);
static int SCALU_METAFN(packet,Packet,add_underlayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,answers)(lua_State *L);
static int SCALU_METAFN(packet,Packet,build)(lua_State *L);
static int SCALU_METAFN(packet,Packet,build_done)(lua_State *L);
static int SCALU_METAFN(packet,Packet,build_padding)(lua_State *L);
static int SCALU_METAFN(packet,Packet,build_ps)(lua_State *L);
static int SCALU_METAFN(packet,Packet,canvas_dump)(lua_State *L);
static int SCALU_METAFN(packet,Packet,clear_cache)(lua_State *L);
static int SCALU_METAFN(packet,Packet,clone_with)(lua_State *L);
static int SCALU_METAFN(packet,Packet,command)(lua_State *L);
static int SCALU_METAFN(packet,Packet,convert_to)(lua_State *L);
static int SCALU_METAFN(packet,Packet,copy)(lua_State *L);
static int SCALU_METAFN(packet,Packet,copy_field_value)(lua_State *L);
static int SCALU_METAFN(packet,Packet,copy_fields_dict)(lua_State *L);
static int SCALU_METAFN(packet,Packet,decode_payload_as)(lua_State *L);
static int SCALU_METAFN(packet,Packet,default_payload_class)(lua_State *L);
static int SCALU_METAFN(packet,Packet,delfieldval)(lua_State *L);
static int SCALU_METAFN(packet,Packet,dissect)(lua_State *L);
static int SCALU_METAFN(packet,Packet,dissection_done)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_build)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_build_payload)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_build_ps)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_dissect)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_dissect_payload)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_init_cached_fields)(lua_State *L);
static int SCALU_METAFN(packet,Packet,do_init_fields)(lua_State *L);
static int SCALU_METAFN(packet,Packet,extract_padding)(lua_State *L);
static int SCALU_METAFN(packet,Packet,firstlayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,fragment)(lua_State *L);
static int SCALU_METAFN(packet,Packet,get_field)(lua_State *L);
static int SCALU_METAFN(packet,Packet,get_field_and_val)(lua_State *L);
static int SCALU_METAFN(packet,Packet,getfieldval)(lua_State *L);
static int SCALU_METAFN(packet,Packet,getlayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,guess_payload_class)(lua_State *L);
static int SCALU_METAFN(packet,Packet,hashret)(lua_State *L);
static int SCALU_METAFN(packet,Packet,haslayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,hide_defaults)(lua_State *L);
static int SCALU_METAFN(packet,Packet,init_fields)(lua_State *L);
static int SCALU_METAFN(packet,Packet,iterpayloads)(lua_State *L);
static int SCALU_METAFN(packet,Packet,lastlayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,layers)(lua_State *L);
static int SCALU_METAFN(packet,Packet,mysummary)(lua_State *L);
static int SCALU_METAFN(packet,Packet,post_build)(lua_State *L);
static int SCALU_METAFN(packet,Packet,post_dissect)(lua_State *L);
static int SCALU_METAFN(packet,Packet,post_dissection)(lua_State *L);
static int SCALU_METAFN(packet,Packet,pre_dissect)(lua_State *L);
static int SCALU_METAFN(packet,Packet,prepare_cached_fields)(lua_State *L);
static int SCALU_METAFN(packet,Packet,remove_underlayer)(lua_State *L);
static int SCALU_METAFN(packet,Packet,self_build)(lua_State *L);
static int SCALU_METAFN(packet,Packet,setfieldval)(lua_State *L);
static int SCALU_METAFN(packet,Packet,show)(lua_State *L);
static int SCALU_METAFN(packet,Packet,show2)(lua_State *L);
static int SCALU_METAFN(packet,Packet,summary)(lua_State *L);
static int SCALU_METAFN(packet,Packet,update_sent_time)(lua_State *L);
static int SCALU_MODFN(packet,Packet)(lua_State *L);

static const struct luaL_Reg lib_scalu_packet[] = {
	SCALU_MODENT(packet,Packet),
	{NULL, NULL}
};

static const struct luaL_Reg cls_Packet[] = {
	SCALU_CLSENT(packet,Packet,convert_packet),
	SCALU_CLSENT(packet,Packet,convert_packets),
	SCALU_CLSENT(packet,Packet,lower_bonds),
	SCALU_CLSENT(packet,Packet,upper_bonds),
	{NULL, NULL}
};

static const struct luaL_Reg idx_Packet[] = {
	SCALU_METAENT(packet,Packet,add_payload),
	SCALU_METAENT(packet,Packet,add_underlayer),
	SCALU_METAENT(packet,Packet,answers),
	SCALU_METAENT(packet,Packet,build),
	SCALU_METAENT(packet,Packet,build_done),
	SCALU_METAENT(packet,Packet,build_padding),
	SCALU_METAENT(packet,Packet,build_ps),
	SCALU_METAENT(packet,Packet,canvas_dump),
	SCALU_METAENT(packet,Packet,clear_cache),
	SCALU_METAENT(packet,Packet,clone_with),
	SCALU_METAENT(packet,Packet,command),
	SCALU_METAENT(packet,Packet,convert_to),
	SCALU_METAENT(packet,Packet,copy),
	SCALU_METAENT(packet,Packet,copy_field_value),
	SCALU_METAENT(packet,Packet,copy_fields_dict),
	SCALU_METAENT(packet,Packet,decode_payload_as),
	SCALU_METAENT(packet,Packet,default_payload_class),
	SCALU_METAENT(packet,Packet,delfieldval),
	SCALU_METAENT(packet,Packet,dissect),
	SCALU_METAENT(packet,Packet,dissection_done),
	SCALU_METAENT(packet,Packet,do_build),
	SCALU_METAENT(packet,Packet,do_build_payload),
	SCALU_METAENT(packet,Packet,do_build_ps),
	SCALU_METAENT(packet,Packet,do_dissect),
	SCALU_METAENT(packet,Packet,do_dissect_payload),
	SCALU_METAENT(packet,Packet,do_init_cached_fields),
	SCALU_METAENT(packet,Packet,do_init_fields),
	SCALU_METAENT(packet,Packet,extract_padding),
	SCALU_METAENT(packet,Packet,firstlayer),
	SCALU_METAENT(packet,Packet,fragment),
	SCALU_METAENT(packet,Packet,get_field),
	SCALU_METAENT(packet,Packet,get_field_and_val),
	SCALU_METAENT(packet,Packet,getfieldval),
	SCALU_METAENT(packet,Packet,getlayer),
	SCALU_METAENT(packet,Packet,guess_payload_class),
	SCALU_METAENT(packet,Packet,hashret),
	SCALU_METAENT(packet,Packet,haslayer),
	SCALU_METAENT(packet,Packet,hide_defaults),
	SCALU_METAENT(packet,Packet,init_fields),
	SCALU_METAENT(packet,Packet,iterpayloads),
	SCALU_METAENT(packet,Packet,lastlayer),
	SCALU_METAENT(packet,Packet,layers),
	SCALU_METAENT(packet,Packet,mysummary),
	SCALU_METAENT(packet,Packet,post_build),
	SCALU_METAENT(packet,Packet,post_dissect),
	SCALU_METAENT(packet,Packet,post_dissection),
	SCALU_METAENT(packet,Packet,pre_dissect),
	SCALU_METAENT(packet,Packet,prepare_cached_fields),
	SCALU_METAENT(packet,Packet,remove_underlayer),
	SCALU_METAENT(packet,Packet,self_build),
	SCALU_METAENT(packet,Packet,setfieldval),
	SCALU_METAENT(packet,Packet,show),
	SCALU_METAENT(packet,Packet,show2),
	SCALU_METAENT(packet,Packet,summary),
	SCALU_METAENT(packet,Packet,update_sent_time),
	{NULL, NULL}
};

static void SCALU_CLS(packet,Packet)(lua_State *L) {
	// t
	lua_pushliteral(L, "Packet"); // t s
	lua_gettable(L, -2); // t f

	lua_newtable(L); // t f t
	lua_pushliteral(L, "__index"); // t f t s
	luaL_newlibtable(L, cls_Packet); // t f t s t
	luaL_setfuncs(L, cls_Packet, 0); // t f t s t
	lua_settable(L, -3); // t f t

	lua_setmetatable(L, -2); // t f
	lua_pop(L, 1); // t
}

static int SCALU_CLSFN(packet,Packet,convert_packet)(lua_State *L) {
	return 0;
}

static int SCALU_CLSFN(packet,Packet,convert_packets)(lua_State *L) {
	return 0;
}

static int SCALU_CLSFN(packet,Packet,lower_bonds)(lua_State *L) {
	return 0;
}

static int SCALU_CLSFN(packet,Packet,upper_bonds)(lua_State *L) {
	return 0;
}


static void SCALU_META(packet,Packet)(lua_State *L) {
	// t t
	lua_pushliteral(L, "__index"); // t t s
	luaL_newlibtable(L, idx_Packet); // t t s t
	luaL_setfuncs(L, idx_Packet, 0); // t t s t
	lua_settable(L, -3); // t t
}

static int SCALU_METAFN(packet,Packet,add_payload)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,add_underlayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,answers)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,build)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,build_done)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,build_padding)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,build_ps)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,canvas_dump)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,clear_cache)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,clone_with)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,command)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,convert_to)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,copy)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,copy_field_value)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,copy_fields_dict)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,decode_payload_as)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,default_payload_class)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,delfieldval)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,dissect)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,dissection_done)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_build)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_build_payload)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_build_ps)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_dissect)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_dissect_payload)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_init_cached_fields)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,do_init_fields)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,extract_padding)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,firstlayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,fragment)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,get_field)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,get_field_and_val)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,getfieldval)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,getlayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,guess_payload_class)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,hashret)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,haslayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,hide_defaults)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,init_fields)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,iterpayloads)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,lastlayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,layers)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,mysummary)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,post_build)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,post_dissect)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,post_dissection)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,pre_dissect)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,prepare_cached_fields)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,remove_underlayer)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,self_build)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,setfieldval)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,show)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,show2)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,summary)(lua_State *L) {
	return 0;
}

static int SCALU_METAFN(packet,Packet,update_sent_time)(lua_State *L) {
	return 0;
}

static int SCALU_MODFN(packet,Packet)(lua_State *L) {
	lua_newtable(L); // t

	if (luaL_newmetatable(L, SCALU_TYPE_PACKET)) { // t t
		SCALU_META(packet,Packet)(L); // t t
	}

	lua_setmetatable(L, -2); // t

	return 1;
}

SCALU_API int SCALU_MOD(packet)(lua_State *L) {
	luaL_newlibtable(L, lib_scalu_packet); // t
	luaL_setfuncs(L, lib_scalu_packet, 0); // t

	SCALU_CLS(packet,Packet)(L); // t

	return 1;
}
