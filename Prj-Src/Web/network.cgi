i pg_header.inc
t <a href="/main_page.cgi">������Ϣ</a>
t <a href="/system.cgi">ϵͳ����</a>
t <a href="/aplication_set.cgi">Ӧ������</a>
t <a class="cur">IP����</a>
t </td><td valign="top">
t <style>
t #m_table{margin:0px 0px 0px 20px;}
t #m_table tr td{padding:5px 30px 5px 30px;background:#fff;}
t #m_table tr td.ti{font-weight:bold;padding:20px 30px 5px 5px;}
t #m_table tr td.val{background:#eee;}
t #m_table tr td.desc{background:#ccc;}
t #m_table tr td.btns{padding:20px 30px 20px 0px;}
t #m_table tr td.btns input{width:80px;height:30px;}
t </style>
t <form name="cgi" action="network.cgi" method="get" onsubmit="return frmsubmit();">
t <table id="m_table" border="0" cellspacing="1">
#t <tr><td nowrap class="ti">�Զ���ȡIP</td><td nowrap>&nbsp;</td></tr>
#c x 1 <tr><td nowrap class="desc">��ǰDHCP״̬</td><td nowrap class="val">%s</td></tr>
#t <tr><td nowrap class="desc">����DHCP</td><td nowrap class="val">
#t <input type="radio" name="radio9" value="yes" id="rad1A" >
#t <label for="rad1A" class="slab">����</label>
#t <input type="radio" name="radio9" checked="checked" value="no" id="rad1B" >
#t <label for="rad1B" class="slab">����</label></td></tr>
t <tr><td nowrap class="ti">����IP����</td><td nowrap>&nbsp;</td></tr>
t <tr><td nowrap class="desc">����IP</td><td nowrap class="val">
c a i <input type="text" name="ip" value="%d.%d.%d.%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="desc">��������</td><td nowrap class="val">
c a m <input type="text" name="msk" value="%d.%d.%d.%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="desc">��������</td><td nowrap class="val">
c a g <input type="text" name="gw" value="%d.%d.%d.%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="ti">������IP����</td><td nowrap>&nbsp;</td></tr>
t <tr><td nowrap class="desc">��������IP</td><td nowrap class="val">
c a s <input type="text" name="sip" value="%d.%d.%d.%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="desc">���������˿�</td><td nowrap class="val">
c a p <input type="text" name="spt" value="%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="desc">�ӷ�����IP</td><td nowrap class="val">
c a w <input type="text" name="sww" value="%d.%d.%d.%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap class="desc">�ӷ������˿�</td><td nowrap class="val">
c a x <input type="text" name="swp" value="%d" size="18" maxlength="18"></td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="ȷ��" id="confirm"/>
t <input type="reset" value="����"/></td></tr></table></form>
t <script>function frmsubmit(){return confirm('ȷ���޸�IP����?');}</script>
i pg_footer.inc
. End of script must be closed with period.