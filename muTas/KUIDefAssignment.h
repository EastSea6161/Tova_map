#pragma once
namespace UIAssignment
{
    const CString UIXAML_A_BACKIMAGE    
        (
        _T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>") 
        _T("  <Border.Background>") 
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>") 
        _T("      <GradientStop Color='#527BA8' Offset='0'/>") 
        _T("      <GradientStop Color='#aac9de' Offset='0.567'/>") 
        _T("    </LinearGradientBrush>")  
        _T("  </Border.Background>")  
        _T("</Border>") 
        );

    const CString UIXAML_A_DEMAND_FALSE
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Select Demand </TextBlock>")
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Stratum(Tijp)</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_DEMAND_DEFAULT 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")    
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Select Demand </TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333'> Stratum(Tijp)</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_DEMAND_SELECTED 
        (
        _T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
        _T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Select Demand </TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366'> Stratum(Tijp)</TextBlock>") 
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_MODEL_DEFAULT 
        (
        _T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_MODEL_DEFAULT 
		(
		_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel  HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도로 통행배정 모형</TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_A_MODEL_SELECTED 
		(
		_T("<Border Width='182' Height='162' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#6d896a' />")
		_T("    <Border Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_A_MODEL_SELECTED 
		(
		_T("<Border Width='182' Height='162' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel  HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 15, 0, 15' FontWeight='Bold' Foreground='#003366'>도로 통행배정 모형</TextBlock>")    
		_T("    <Border Width='178' Height='1' Background='#6d896a' />")
		_T("    <Border Width='178' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_INPUT_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Trip Matrix </TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_INPUT_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 수단별 기종점 통행자료 </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);


    const CString
        UIXAML_A_INPUT_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Trip Matrix </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_INPUT_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 수단별 기종점 통행자료 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_INPUT_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Trip Matrix </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_INPUT_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'> 수단별 기종점 통행자료 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_RUN_OPTIONS_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Run Options </TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_RUN_OPTIONS_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>실행 옵션 설정</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_RUN_OPTIONS_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Run Options </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_RUN_OPTIONS_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>실행 옵션 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_RUN_OPTIONS_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Run Options </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_RUN_OPTIONS_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>실행 옵션 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_DEFINE_VDF_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Volume Delay </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_DEFINE_VDF_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 통행지체함수 설정 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_DEFINE_VDF_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Volume Delay </TextBlock>")
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_DEFINE_VDF_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 통행지체함수 설정 </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_DEFINE_VDF_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Volume Delay </TextBlock>")
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366' HorizontalAlignment='Center'> Function</TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_DEFINE_VDF_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'> 통행지체함수 설정 </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_TRANSIT_ASSIGN_OPTIONS_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Transit Assign</TextBlock>")    
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Options</TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_TRANSIT_ASSIGN_OPTIONS_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Transit Assign </TextBlock>") 
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Options</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_TRANSIT_ASSIGN_OPTIONS_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Transit Assign </TextBlock>")  
        _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Options</TextBlock>")  
        _T("  </StackPanel>")
        _T("</Border>")
        );

    const CString
        UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Friction Factor </TextBlock>")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>점진적 배정법</TextBlock>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>분할배정 비율 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
         _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Friction Factor </TextBlock>")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>점진적 배정법</TextBlock>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>분할배정 비율 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_TRANSIT_ODDEMAND_SHARE_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
       _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Friction Factor </TextBlock>")
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_TRANSIT_ODDEMAND_SHARE_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>점진적 배정법</TextBlock>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>분할배정 비율 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_STOPPING_CRITERIA_FALSE
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Stopping Criteria </TextBlock>")   
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_STOPPING_CRITERIA_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>반복수행 종료조건</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_STOPPING_CRITERIA_DEFAULT
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Stopping Criteria </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_STOPPING_CRITERIA_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕'  TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_STOPPING_CRITERIA_SELECTED 
        (
        _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Stopping Criteria </TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_STOPPING_CRITERIA_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_A_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_A_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);


	const CString
		UIXAML_A_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_A_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);

    const CString
        UIXAML_A_OUTPUT_SELECTED 
        (
        _T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#46688D' />")
        _T("    <Border Height='1' Background='#7AA5CF' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
	const CString
		KR_UIXAML_A_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
}


const CString
	UIXAML_A_VIEWTABLE_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	//_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW TABLE </TextBlock>")
	_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Result View </TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);
const CString
	KR_UIXAML_A_VIEWTABLE_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>결과보기</TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_A_VIEWTABLE_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	//_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW TABLE </TextBlock>")
	_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Result View </TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);
const CString
	KR_UIXAML_A_VIEWTABLE_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>결과보기</TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);


const CString
	UIXAML_A_VIEWLOG_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW LOG </TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);
const CString
	KR_UIXAML_A_VIEWLOG_FALSE
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>로그 파일 보기</TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_A_VIEWLOG_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='18, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW LOG </TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);
const CString
	KR_UIXAML_A_VIEWLOG_DEFAULT
	(
	_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
	_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>로그 파일 보기</TextBlock>")
	_T("  </StackPanel>")
	_T("</Border>")
	);

//CString BUILD_MODEL_UIXAML(CWnd * pPlaceholder, CString xamlString)
//{
//	if (!pPlaceholder || !::IsWindow(pPlaceholder->GetSafeHwnd()))
//		return _T("");
//
//	UINT dpi = GetDpiForWindow(*pPlaceholder);
//
//	CRect rc;
//	pPlaceholder->GetWindowRect(&rc);
//	pPlaceholder->GetParent()->ScreenToClient(&rc);
//
//	int w = rc.Width() - 4;
//	int h = rc.Height() - 4;
//
//	CString templateXaml = xamlString;
//	
//	// Width 치환
//	int pos = templateXaml.Find(_T("Width='"));
//	if (pos >= 0)
//	{
//		int endPos = templateXaml.Find(_T("'"), pos + 7);
//		if (endPos > pos)
//			templateXaml.Delete(pos + 7, endPos - (pos + 7));
//		templateXaml.Insert(pos + 7, _T("%d"));
//	}
//
//	// Height 치환
//	pos = templateXaml.Find(_T("Height='"));
//	if (pos >= 0)
//	{
//		int endPos = templateXaml.Find(_T("'"), pos + 8);
//		if (endPos > pos)
//			templateXaml.Delete(pos + 8, endPos - (pos + 8));
//		templateXaml.Insert(pos + 8, _T("%d"));
//	}
//
//	// FontSize 치환
//	int fontSizeValue = 0;
//	pos = templateXaml.Find(_T("TextBlock.FontSize='"));
//	if (pos >= 0)
//	{
//		int startPos = pos + 20;
//		int endPos = templateXaml.Find(_T("'"), startPos);
//		CString fontSizeStr = templateXaml.Mid(startPos, endPos - startPos);
//		fontSizeValue = _ttoi(fontSizeStr);
//
//		// DPI 비율 적용
//		fontSizeValue = (int)(fontSizeValue * (dpi / 96.0f));
//
//		// 치환
//		templateXaml.Delete(startPos, endPos - startPos);
//		templateXaml.Insert(startPos, _T("%d"));
//	}
//
//	CString finalXaml;
//	finalXaml.Format(templateXaml, w, h, fontSizeValue);
//
//	return finalXaml;
//}