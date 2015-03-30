namespace PokemonOU
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.attacker = new System.Windows.Forms.ComboBox();
            this.defender = new System.Windows.Forms.ComboBox();
            this.calculate = new System.Windows.Forms.Button();
            this.basePower = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.damage = new System.Windows.Forms.Label();
            this.defenderType = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.mod_atk = new System.Windows.Forms.ComboBox();
            this.mod_spatk = new System.Windows.Forms.ComboBox();
            this.mod_spdef = new System.Windows.Forms.ComboBox();
            this.mod_def = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.moveCat = new System.Windows.Forms.ComboBox();
            this.def_type1 = new System.Windows.Forms.Label();
            this.atk_type1 = new System.Windows.Forms.Label();
            this.atk_stat = new System.Windows.Forms.Label();
            this.def_stat = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.atk_type2 = new System.Windows.Forms.Label();
            this.def_type2 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.moveType = new System.Windows.Forms.ComboBox();
            this.label17 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(12, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 18);
            this.label1.TabIndex = 0;
            this.label1.Text = "Attacker:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(322, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 18);
            this.label2.TabIndex = 1;
            this.label2.Text = "Defender:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(12, 158);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(108, 18);
            this.label3.TabIndex = 2;
            this.label3.Text = "Attack Modifier:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(12, 198);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(130, 18);
            this.label4.TabIndex = 2;
            this.label4.Text = "Sp. Attack Modifier:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(322, 154);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(121, 18);
            this.label5.TabIndex = 2;
            this.label5.Text = "Defense Modifier:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(322, 194);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(143, 18);
            this.label6.TabIndex = 2;
            this.label6.Text = "Sp. Defense Modifier:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(12, 99);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(58, 18);
            this.label7.TabIndex = 3;
            this.label7.Text = "Type(s):";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(322, 99);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(58, 18);
            this.label8.TabIndex = 4;
            this.label8.Text = "Type(s):";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(322, 242);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(84, 18);
            this.label9.TabIndex = 5;
            this.label9.Text = "Base Power:";
            // 
            // attacker
            // 
            this.attacker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.attacker.FormattingEnabled = true;
            this.attacker.Location = new System.Drawing.Point(96, 20);
            this.attacker.Name = "attacker";
            this.attacker.Size = new System.Drawing.Size(118, 24);
            this.attacker.TabIndex = 6;
            this.attacker.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // defender
            // 
            this.defender.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.defender.FormattingEnabled = true;
            this.defender.Location = new System.Drawing.Point(410, 19);
            this.defender.Name = "defender";
            this.defender.Size = new System.Drawing.Size(126, 24);
            this.defender.TabIndex = 7;
            this.defender.SelectedIndexChanged += new System.EventHandler(this.defender_SelectedIndexChanged);
            // 
            // calculate
            // 
            this.calculate.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.calculate.Location = new System.Drawing.Point(347, 346);
            this.calculate.Name = "calculate";
            this.calculate.Size = new System.Drawing.Size(141, 32);
            this.calculate.TabIndex = 8;
            this.calculate.Text = "Calculate Damage";
            this.calculate.UseVisualStyleBackColor = true;
            this.calculate.Click += new System.EventHandler(this.calculate_Click);
            // 
            // basePower
            // 
            this.basePower.Location = new System.Drawing.Point(415, 242);
            this.basePower.Name = "basePower";
            this.basePower.Size = new System.Drawing.Size(80, 22);
            this.basePower.TabIndex = 9;
            this.basePower.Text = "0";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(344, 381);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(62, 18);
            this.label10.TabIndex = 10;
            this.label10.Text = "Damage:";
            // 
            // damage
            // 
            this.damage.AutoSize = true;
            this.damage.Location = new System.Drawing.Point(415, 381);
            this.damage.Name = "damage";
            this.damage.Size = new System.Drawing.Size(75, 17);
            this.damage.TabIndex = 11;
            this.damage.Text = "<damage>";
            // 
            // defenderType
            // 
            this.defenderType.AutoSize = true;
            this.defenderType.Location = new System.Drawing.Point(440, 47);
            this.defenderType.Name = "defenderType";
            this.defenderType.Size = new System.Drawing.Size(0, 17);
            this.defenderType.TabIndex = 13;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(322, 126);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(42, 18);
            this.label11.TabIndex = 14;
            this.label11.Text = "Stats:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(12, 126);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(42, 18);
            this.label12.TabIndex = 14;
            this.label12.Text = "Stats:";
            // 
            // mod_atk
            // 
            this.mod_atk.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mod_atk.FormattingEnabled = true;
            this.mod_atk.Items.AddRange(new object[] {
            "-6",
            "-5",
            "-4",
            "-3",
            "-2",
            "-1",
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"});
            this.mod_atk.Location = new System.Drawing.Point(154, 152);
            this.mod_atk.Name = "mod_atk";
            this.mod_atk.Size = new System.Drawing.Size(60, 24);
            this.mod_atk.TabIndex = 16;
            // 
            // mod_spatk
            // 
            this.mod_spatk.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mod_spatk.FormattingEnabled = true;
            this.mod_spatk.Items.AddRange(new object[] {
            "-6",
            "-5",
            "-4",
            "-3",
            "-2",
            "-1",
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"});
            this.mod_spatk.Location = new System.Drawing.Point(154, 196);
            this.mod_spatk.Name = "mod_spatk";
            this.mod_spatk.Size = new System.Drawing.Size(60, 24);
            this.mod_spatk.TabIndex = 16;
            // 
            // mod_spdef
            // 
            this.mod_spdef.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mod_spdef.FormattingEnabled = true;
            this.mod_spdef.Items.AddRange(new object[] {
            "-6",
            "-5",
            "-4",
            "-3",
            "-2",
            "-1",
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"});
            this.mod_spdef.Location = new System.Drawing.Point(476, 191);
            this.mod_spdef.Name = "mod_spdef";
            this.mod_spdef.Size = new System.Drawing.Size(60, 24);
            this.mod_spdef.TabIndex = 16;
            // 
            // mod_def
            // 
            this.mod_def.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mod_def.FormattingEnabled = true;
            this.mod_def.Items.AddRange(new object[] {
            "-6",
            "-5",
            "-4",
            "-3",
            "-2",
            "-1",
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"});
            this.mod_def.Location = new System.Drawing.Point(476, 151);
            this.mod_def.Name = "mod_def";
            this.mod_def.Size = new System.Drawing.Size(60, 24);
            this.mod_def.TabIndex = 16;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(322, 280);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(106, 18);
            this.label13.TabIndex = 17;
            this.label13.Text = "Move Category:";
            // 
            // moveCat
            // 
            this.moveCat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.moveCat.FormattingEnabled = true;
            this.moveCat.Items.AddRange(new object[] {
            "Physical",
            "Special"});
            this.moveCat.Location = new System.Drawing.Point(435, 277);
            this.moveCat.Name = "moveCat";
            this.moveCat.Size = new System.Drawing.Size(113, 24);
            this.moveCat.TabIndex = 16;
            // 
            // def_type1
            // 
            this.def_type1.AutoSize = true;
            this.def_type1.Location = new System.Drawing.Point(407, 71);
            this.def_type1.Name = "def_type1";
            this.def_type1.Size = new System.Drawing.Size(112, 17);
            this.def_type1.TabIndex = 18;
            this.def_type1.Text = "<defender type>";
            // 
            // atk_type1
            // 
            this.atk_type1.AutoSize = true;
            this.atk_type1.Location = new System.Drawing.Point(79, 72);
            this.atk_type1.Name = "atk_type1";
            this.atk_type1.Size = new System.Drawing.Size(106, 17);
            this.atk_type1.TabIndex = 18;
            this.atk_type1.Text = "<attacker type>";
            // 
            // atk_stat
            // 
            this.atk_stat.AutoSize = true;
            this.atk_stat.Location = new System.Drawing.Point(83, 126);
            this.atk_stat.Name = "atk_stat";
            this.atk_stat.Size = new System.Drawing.Size(102, 17);
            this.atk_stat.TabIndex = 18;
            this.atk_stat.Text = "<attacker stat>";
            // 
            // def_stat
            // 
            this.def_stat.AutoSize = true;
            this.def_stat.Location = new System.Drawing.Point(411, 126);
            this.def_stat.Name = "def_stat";
            this.def_stat.Size = new System.Drawing.Size(108, 17);
            this.def_stat.TabIndex = 18;
            this.def_stat.Text = "<defender stat>";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(12, 72);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(58, 18);
            this.label14.TabIndex = 3;
            this.label14.Text = "Type(s):";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(322, 73);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(58, 18);
            this.label15.TabIndex = 4;
            this.label15.Text = "Type(s):";
            // 
            // atk_type2
            // 
            this.atk_type2.AutoSize = true;
            this.atk_type2.Location = new System.Drawing.Point(79, 99);
            this.atk_type2.Name = "atk_type2";
            this.atk_type2.Size = new System.Drawing.Size(106, 17);
            this.atk_type2.TabIndex = 18;
            this.atk_type2.Text = "<attacker type>";
            // 
            // def_type2
            // 
            this.def_type2.AutoSize = true;
            this.def_type2.Location = new System.Drawing.Point(407, 99);
            this.def_type2.Name = "def_type2";
            this.def_type2.Size = new System.Drawing.Size(112, 17);
            this.def_type2.TabIndex = 18;
            this.def_type2.Text = "<defender type>";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Calibri", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(322, 319);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(81, 18);
            this.label16.TabIndex = 19;
            this.label16.Text = "Move Type:";
            // 
            // moveType
            // 
            this.moveType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.moveType.FormattingEnabled = true;
            this.moveType.Items.AddRange(new object[] {
            "Normal",
            "Fighting",
            "Flying",
            "Poison",
            "Ground",
            "Rock",
            "Bug",
            "Ghost",
            "Steel",
            "Fire",
            "Water",
            "Grass",
            "Electric",
            "Psychic",
            "Ice",
            "Dragon",
            "Dark",
            "Fairy"});
            this.moveType.Location = new System.Drawing.Point(435, 316);
            this.moveType.Name = "moveType";
            this.moveType.Size = new System.Drawing.Size(113, 24);
            this.moveType.TabIndex = 16;
            this.moveType.SelectedIndexChanged += new System.EventHandler(this.moveType_SelectedIndexChanged);
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(12, 242);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(271, 119);
            this.label17.TabIndex = 20;
            this.label17.Text = resources.GetString("label17.Text");
            this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(628, 418);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.def_stat);
            this.Controls.Add(this.atk_stat);
            this.Controls.Add(this.atk_type2);
            this.Controls.Add(this.atk_type1);
            this.Controls.Add(this.def_type2);
            this.Controls.Add(this.def_type1);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.moveType);
            this.Controls.Add(this.moveCat);
            this.Controls.Add(this.mod_def);
            this.Controls.Add(this.mod_spdef);
            this.Controls.Add(this.mod_spatk);
            this.Controls.Add(this.mod_atk);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.defenderType);
            this.Controls.Add(this.damage);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.basePower);
            this.Controls.Add(this.calculate);
            this.Controls.Add(this.defender);
            this.Controls.Add(this.attacker);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Pokemon OU Damage Calculator";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox attacker;
        private System.Windows.Forms.ComboBox defender;
        private System.Windows.Forms.Button calculate;
        private System.Windows.Forms.TextBox basePower;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label damage;
        private System.Windows.Forms.Label defenderType;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox mod_atk;
        private System.Windows.Forms.ComboBox mod_spatk;
        private System.Windows.Forms.ComboBox mod_spdef;
        private System.Windows.Forms.ComboBox mod_def;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox moveCat;
        private System.Windows.Forms.Label def_type1;
        private System.Windows.Forms.Label atk_type1;
        private System.Windows.Forms.Label atk_stat;
        private System.Windows.Forms.Label def_stat;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label atk_type2;
        private System.Windows.Forms.Label def_type2;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.ComboBox moveType;
        private System.Windows.Forms.Label label17;

    }
}

